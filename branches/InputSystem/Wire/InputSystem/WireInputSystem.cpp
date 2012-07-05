#include "WireInputSystem.h"

namespace Wire
{

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

	if (pDevice->IsDerived(MainInputDevice::TYPE))
	{
		mMainDevices.push_back(static_cast<MainInputDevice*>(pDevice));
	}

	else if (pDevice->IsDerived(InputDeviceExtension::TYPE))
	{
		mDeviceExtensions.push_back(static_cast<InputDeviceExtension*>(pDevice));
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


void InputSystem::RemoveListener(InputSystemListener* pListener)
{
	// TODO:
}

}