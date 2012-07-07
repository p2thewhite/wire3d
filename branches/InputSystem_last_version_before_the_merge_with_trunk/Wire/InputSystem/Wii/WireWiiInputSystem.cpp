#include "WireWiiInputSystem.h"
#include "WireWiiMote.h"
#include "WireNunchuk.h"
#include "WireApplication.h"
#include "WireRenderer.h"
#include "WireMemory.h"
#include <wiiuse/wpad.h>

namespace Wire
{

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
	WPAD_Shutdown();
}

void WiiInputSystem::AfterDevicesDiscovery()
{
	Renderer* pRenderer = Application::GetApplication()->GetRenderer();

	for (UInt i = 0; i < GetMainDevicesCount(); i++)
	{
		WiiMote* pWiiMote = static_cast<WiiMote*>(mMainDevices[i]);
		// TODO: what about the x and y offsets?
		WPAD_SetVRes(pWiiMote->GetChannel(), pRenderer->GetWidth(), pRenderer->GetHeight());
	}
}

WiiInputDataBuffer* WiiInputSystem::GetChannelDataBuffer(UInt channel)
{
	WiiInputDataBuffer* pDataBuffer = mDataBufferByChannel[channel];

	if (pDataBuffer == NULL)
	{
		pDataBuffer = WIRE_NEW WiiInputDataBuffer();
	}

	return pDataBuffer;
}

void WiiInputSystem::Capture()
{
	WPAD_ReadPending(WPAD_CHAN_ALL,	&WiiInputSystem::ReadWPADPendingEventsCallback);

	for (UInt i = 0; i < GetMainDevicesCount(); i++)
	{
		WiiMote* pWiiMote = static_cast<WiiMote*>(mMainDevices[i]);
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
		WiiMote* pWiiMote =	static_cast<WiiMote*>(mMainDevices[i]);

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
		System::Assert("Unsupported wiimote expansion found.", "WireWiiInputSystem.cpp", 138);
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

}