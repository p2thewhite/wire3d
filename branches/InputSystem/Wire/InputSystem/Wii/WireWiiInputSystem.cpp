// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireWiiInputSystem.h"
#include "WireWiiMote.h"
#include "WireNunchuk.h"
#include "WireApplication.h"
#include "WireMemory.h"
#include <wiiuse/wpad.h>

using namespace Wire;

const UInt WiiInputSystem::FIRST_CHANNEL = 0;
const UInt WiiInputSystem::LAST_CHANNEL = 3;
UInt WiiInputSystem::s_mEventCounter = 0;

WiiInputSystem::WiiInputSystem() :
	mLastDiscoveredChannel(-1),
	mChanged(false)
{
	PAD_Init();
	WPAD_Init();
}

WiiInputSystem::~WiiInputSystem()
{
	THashTable<Int, WiiInputDataBuffer*>::Iterator it(&mDataBufferByChannel);
	Int key;
	for (WiiInputDataBuffer** pValue = it.GetFirst(&key); pValue; pValue = it.GetNext(&key))
	{
		WIRE_ASSERT(*pValue);
		WIRE_DELETE *pValue;
	}

	WPAD_Shutdown();
}

void WiiInputSystem::AfterDevicesDiscovery()
{
	Application* pApp = Application::GetApplication();
	if (!pApp)
	{
		return;
	}

	for (UInt i = 0; i < GetMainDevicesCount(); i++)
	{
		WiiMote* pWiiMote = DynamicCast<WiiMote>(mMainDevices[i]);
		WIRE_ASSERT(pWiiMote /* MainInputDevice is not WiiMote */);
		if (!pWiiMote)
		{
			continue;
		}

		// TODO: what about the x and y offsets?
		WPAD_SetVRes(pWiiMote->GetChannel(), pApp->GetWidth(), pApp->GetHeight());
	}
}

WiiInputDataBuffer* WiiInputSystem::GetChannelDataBuffer(UInt channel)
{
	WiiInputDataBuffer** pValue = mDataBufferByChannel.Find(channel);

	if (pValue == NULL)
	{
		WiiInputDataBuffer* pDataBuffer = WIRE_NEW WiiInputDataBuffer();
		mDataBufferByChannel.Insert(channel, pDataBuffer);
		return pDataBuffer;
	}

	return *pValue;
}

void WiiInputSystem::Capture()
{
	WPAD_ReadPending(WPAD_CHAN_ALL,	&WiiInputSystem::ReadWPADPendingEventsCallback);

	for (UInt i = 0; i < GetMainDevicesCount(); i++)
	{
		WiiMote* pWiiMote = DynamicCast<WiiMote>(mMainDevices[i]);
		WIRE_ASSERT(pWiiMote /* MainInputDevice is not WiiMote */);
		if (!pWiiMote)
		{
			continue;
		}

		UInt channel = pWiiMote->GetChannel();
		WiiInputDataBuffer* pChannelDataBuffer = GetChannelDataBuffer(pWiiMote->GetChannel());
		pChannelDataBuffer->SetData(WPAD_Data(channel));
		pWiiMote->SetDataBuffer(pChannelDataBuffer);

		// FIXME: shouldn't it be at WiiMote::SetDataBuffer(..)?
		for (UInt j = 0; j < pWiiMote->GetExtensionsCount(); j++)
		{
			InputDeviceExtension* pExtension = const_cast<InputDeviceExtension*>(pWiiMote->GetExtension(j));
			pExtension->SetDataBuffer(pChannelDataBuffer);
		}
	}
}

void WiiInputSystem::UpdateCurrentlyConnectedChannels()
{
	for (UInt i = 0; i < GetMainDevicesCount(); i++)
	{
		WiiMote* pWiiMote =	DynamicCast<WiiMote>(mMainDevices[i]);
		WIRE_ASSERT(pWiiMote /* MainInputDevice is not WiiMote */);
		if (!pWiiMote)
		{
			continue;
		}

		// if it's not connected anymore, remove it
		if (!IsWiiMoteConnectedToChannel(pWiiMote->GetChannel()))
		{
			RemoveDevice(pWiiMote);
			continue;
		}

		if (pWiiMote->GetExtensionsCount() == 0)
		{
			DiscoverWiiMoteExpansions(pWiiMote);
		}
	}
}

void WiiInputSystem::ProbePreviouslyDisconnectedChannels()
{
	for (UInt channel = mLastDiscoveredChannel + 1; channel <= LAST_CHANNEL;	channel++)
	{
		if (!IsWiiMoteConnectedToChannel(channel))
		{
			continue;
		}

		// register wiimote's expected data format
		WPAD_SetDataFormat(channel, WPAD_FMT_BTNS_ACC_IR);

		// create and add the wiimote
		WiiMote* pWiiMote = WIRE_NEW WiiMote(channel);
		pWiiMote->SetUp();
		AddDevice(pWiiMote);
		DiscoverWiiMoteExpansions(pWiiMote);
		mLastDiscoveredChannel = channel;
		mChanged = true;
	}
}

void WiiInputSystem::NotifyDevicesChangeIfNecessary()
{
	if (mChanged)
	{
		NotifyDevicesChange();
		mChanged = false;
	}
}

void WiiInputSystem::DoDevicesDiscovery()
{
	UpdateCurrentlyConnectedChannels();
	ProbePreviouslyDisconnectedChannels();
	NotifyDevicesChangeIfNecessary();
}

void WiiInputSystem::DiscoverWiiMoteExpansions(WiiMote* pWiiMote)
{
	expansion_t data;
	WPAD_Expansion(pWiiMote->GetChannel(), &data);

	if (data.type == WPAD_EXP_NONE)
	{
		return;
	}

	InputDeviceExtension* pWiiMoteExpansion;

	switch (data.type)
	{
	case WPAD_EXP_NUNCHUK:
		pWiiMoteExpansion = WIRE_NEW Nunchuk(pWiiMote);
		pWiiMote->AddExtension(pWiiMoteExpansion);
		pWiiMoteExpansion->SetUp();
		break;

	default:
		WIRE_ASSERT(false /* Unsupported wiimote expansion found */);
		return;
	}

	mChanged = true;
}

Bool WiiInputSystem::IsWiiMoteConnectedToChannel(UInt channel)
{
	UInt type;
	return (WPAD_Probe(channel, &type) == WPAD_ERR_NONE);
}

void WiiInputSystem::ReadWPADPendingEventsCallback(Int channel, const WPADData* pData)
{
	WiiInputSystem::s_mEventCounter++;
}
