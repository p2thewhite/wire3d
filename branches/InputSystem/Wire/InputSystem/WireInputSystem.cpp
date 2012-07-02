#include "WireInputSystem.h"

namespace Wire
{

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::AfterInputDeviceDiscovery()
{
}

void InputSystem::BeforeInputDeviceDiscovery()
{
}

void InputSystem::DiscoverInputDevices()
{
	BeforeInputDeviceDiscovery();

	mpInputDeviceDiscoveryStrategy->Discover();

	AfterInputDeviceDiscovery();
}

const InputDeviceExtension* InputSystem::GetInputDeviceExtension(UInt index) const
{
	return mpInputDeviceDiscoveryStrategy->GetInputDeviceExtensions()[index];
}

UInt InputSystem::GetInputDeviceExtensionsCount() const
{
	return mpInputDeviceDiscoveryStrategy->GetInputDeviceExtensions().size();
}

const MainInputDevice* InputSystem::GetMainInputDevice(UInt index) const
{
	return mpInputDeviceDiscoveryStrategy->GetMainInputDevices()[index];
}

UInt InputSystem::GetMainInputDevicesCount() const
{
	return mpInputDeviceDiscoveryStrategy->GetMainInputDevices().size();
}

const InputDevice* InputSystem::GetInputDevice(UInt index) const
{
	return mpInputDeviceDiscoveryStrategy->GetInputDevices()[index];
}

UInt InputSystem::GetInputDevicesCount() const
{
	return mpInputDeviceDiscoveryStrategy->GetInputDevices().size();
}

void InputSystem::AddInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pListener)
{
	mpInputDeviceDiscoveryStrategy->AddListener(pListener);
}

void InputSystem::RemoveInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pListener)
{
	mpInputDeviceDiscoveryStrategy->RemoveListener(pListener);
}

}