// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireInputSystem.h"
#include <algorithm>

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
	return mDeviceExtensions.size();
}

const MainInputDevice* InputSystem::GetMainDevice(UInt index) const
{
	return mMainDevices[index];
}

UInt InputSystem::GetMainDevicesCount() const
{
	return mMainDevices.size();
}

const InputDevice* InputSystem::GetDevice(UInt index) const
{
	return mDevices[index];
}

UInt InputSystem::GetDevicesCount() const
{
	return mDevices.size();
}

void InputSystem::AddDevice(InputDevice* pDevice)
{
	mDevices.push_back(pDevice);

	
	MainInputDevice* pMainDevice = DynamicCast<MainInputDevice>(pDevice);
	if (pMainDevice)
	{
		mMainDevices.push_back(pMainDevice);
		for (UInt i = 0; i < pMainDevice->GetExtensionsCount(); i++)
		{
			AddDevice(const_cast<InputDeviceExtension*>(pMainDevice->GetExtension(i)));
		}
	}

	InputDeviceExtension* pInputDeviceExtension = DynamicCast<InputDeviceExtension>(pDevice);
	if (pInputDeviceExtension)
	{
		mDeviceExtensions.push_back(pInputDeviceExtension);
	}
}

void InputSystem::RemoveDevice(InputDevice* pDevice)
{
	mDevices.erase(std::find(mDevices.begin(), mDevices.end(), pDevice));

	if (pDevice->IsDerived(MainInputDevice::TYPE))
	{
		MainInputDevice* pMainDevice = static_cast<MainInputDevice*>(pDevice);
		mMainDevices.erase(std::find(mMainDevices.begin(), mMainDevices.end(), pMainDevice));
		for (UInt i = 0; i < pMainDevice->GetExtensionsCount(); i++)
		{
			RemoveDevice(const_cast<InputDeviceExtension*>(pMainDevice->GetExtension(i)));
		}
	}

	else if (pDevice->IsDerived(InputDeviceExtension::TYPE))
	{
		mDeviceExtensions.erase(std::find(mDeviceExtensions.begin(), mDeviceExtensions.end(), static_cast<InputDeviceExtension*>(pDevice)));
	}
}

void InputSystem::AddListener(InputSystemListener* pListener)
{
	mListeners.push_back(pListener);
}

void InputSystem::NotifyDevicesChange()
{
	std::vector<InputSystemListener*>::const_iterator i = mListeners.begin();

	while (i != mListeners.end())
	{
		(*i)->OnDevicesChange();
		i++;
	}
}


void InputSystem::RemoveListener(InputSystemListener*)
{
	// TODO:
}
