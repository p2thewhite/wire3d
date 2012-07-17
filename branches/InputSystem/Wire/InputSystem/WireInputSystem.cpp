// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireInputSystem.h"

using namespace Wire;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::AfterDevicesDiscovery()
{
}

void InputSystem::BeforeDevicesDiscovery()
{
}

/************************************************************************/
/*                       Template method                                */
/************************************************************************/
void InputSystem::DiscoverDevices()
{
	BeforeDevicesDiscovery();
	DoDevicesDiscovery();
	AfterDevicesDiscovery();
}

const InputDeviceExtension* InputSystem::GetDeviceExtension(UInt index) const
{
	return mDeviceExtensions[index];
}

UInt InputSystem::GetDeviceExtensionsCount() const
{
	return mDeviceExtensions.GetQuantity();
}

const MainInputDevice* InputSystem::GetMainDevice(UInt index) const
{
	return mMainDevices[index];
}

UInt InputSystem::GetMainDevicesCount() const
{
	return mMainDevices.GetQuantity();
}

const InputDevice* InputSystem::GetDevice(UInt index) const
{
	return mDevices[index];
}

UInt InputSystem::GetDevicesCount() const
{
	return mDevices.GetQuantity();
}

void InputSystem::AddDevice(InputDevice* pDevice)
{
	mDevices.Append(pDevice);
	
	MainInputDevice* pMainDevice = DynamicCast<MainInputDevice>(pDevice);
	if (pMainDevice)
	{
		mMainDevices.Append(pMainDevice);
		for (UInt i = 0; i < pMainDevice->GetExtensionsCount(); i++)
		{
			AddDevice(const_cast<InputDeviceExtension*>(pMainDevice->GetExtension(i)));
		}
	}

	InputDeviceExtension* pInputDeviceExtension = DynamicCast<InputDeviceExtension>(pDevice);
	if (pInputDeviceExtension)
	{
		mDeviceExtensions.Append(pInputDeviceExtension);
	}
}

void InputSystem::RemoveDevice(InputDevice* pDevice)
{
	mDevices.Remove(pDevice);

	if (pDevice->IsDerived(MainInputDevice::TYPE))
	{
		MainInputDevice* pMainDevice = static_cast<MainInputDevice*>(pDevice);
		mMainDevices.Remove(pMainDevice);
		for (UInt i = 0; i < pMainDevice->GetExtensionsCount(); i++)
		{
			RemoveDevice(const_cast<InputDeviceExtension*>(pMainDevice->GetExtension(i)));
		}
	}

	else if (pDevice->IsDerived(InputDeviceExtension::TYPE))
	{
		mDeviceExtensions.Remove(static_cast<InputDeviceExtension*>(pDevice));
	}
}

void InputSystem::AddListener(InputSystemListener* pListener)
{
	mListeners.Append(pListener);
}

void InputSystem::NotifyDevicesChange()
{
	for (UInt i = 0; i < mListeners.GetQuantity(); i++)
	{
		WIRE_ASSERT(mListeners[i]);
		mListeners[i]->OnDevicesChange();
	}
}

void InputSystem::RemoveListener(InputSystemListener*)
{
	// TODO:
}
