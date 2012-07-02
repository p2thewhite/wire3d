#include "WireInputDeviceDiscoveryStrategy.h"

namespace Wire
{

InputDeviceDiscoveryStrategy::InputDeviceDiscoveryStrategy()
{
}

InputDeviceDiscoveryStrategy::~InputDeviceDiscoveryStrategy()
{
}

void InputDeviceDiscoveryStrategy::AddInputDevice(InputDevice* pInputDevice)
{
	mInputDevices.push_back(pInputDevice);

	if (pInputDevice->IsDerived(MainInputDevice::TYPE))
	{
		mMainInputDevices.push_back(static_cast<MainInputDevice*>(pInputDevice));
	}

	else if (pInputDevice->IsDerived(InputDeviceExtension::TYPE))
	{
		mInputDeviceExtensions.push_back(static_cast<InputDeviceExtension*>(pInputDevice));
	}
}

void InputDeviceDiscoveryStrategy::AddListener(InputDeviceDiscoveryListener* pListener)
{
	mListeners.push_back(pListener);
}

const std::vector<InputDeviceExtension*>& InputDeviceDiscoveryStrategy::GetInputDeviceExtensions() const
{
	return mInputDeviceExtensions;
}

const std::vector<InputDevice*>& InputDeviceDiscoveryStrategy::GetInputDevices() const
{
	return mInputDevices;
}

const std::vector<MainInputDevice*>& InputDeviceDiscoveryStrategy::GetMainInputDevices() const
{
	return mMainInputDevices;
}

void InputDeviceDiscoveryStrategy::NotifyListeners()
{
	std::vector<InputDeviceDiscoveryListener*>::const_iterator i = mListeners.begin();

	while (i != mListeners.end())
	{
		(*i)->Update();
		i++;
	}
}


void InputDeviceDiscoveryStrategy::RemoveListener(InputDeviceDiscoveryListener* pListener)
{
	// TODO:
}

}