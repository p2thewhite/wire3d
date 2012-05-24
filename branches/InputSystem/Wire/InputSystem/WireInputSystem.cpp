#include "WireInputSystem.h"
#include "WireInputDevice.h"
#include "WireMemory.h"
#include "WireFallbackVirtualInputDevice.h"

namespace Wire
{

InputSystem::InputSystem()
{
	mpFallbackVirtualInputDevice = WIRE_NEW FallbackVirtualInputDevice();
}

InputSystem::~InputSystem()
{
	WIRE_DELETE mpFallbackVirtualInputDevice;
}

Bool InputSystem::DiscoverInputDevices()
{
	// Template method.
	BeforeInputDevicesDiscovery();

	mpDevicesDiscoveryStrategy->DiscoverVirtualInputDevices();

	// Template method.
	AfterInputDevicesDiscovery();

	return true;
}

void InputSystem::BeforeInputDevicesDiscovery()
{
	// Do nothing.
}

void InputSystem::AfterInputDevicesDiscovery()
{
	// Do nothing.
}

void InputSystem::Capture()
{
	//const TArray<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();
	const vector<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();

	//for (UInt i = 0; i < virtualInputDevices.GetQuantity(); i++)
	for (UInt i = 0; i < virtualInputDevices.size(); i++)
	{
		VirtualInputDevice* pVirtualInputDevice = virtualInputDevices[i];
		pVirtualInputDevice->Capture();
	}
}

}
