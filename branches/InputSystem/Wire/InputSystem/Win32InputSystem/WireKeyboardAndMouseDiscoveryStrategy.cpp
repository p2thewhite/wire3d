#include "WireKeyboardAndMouseDiscoveryStrategy.h"
#include "WireInputSystem.h"
#include "WireKeyboardAndMouse.h"

namespace Wire
{

KeyboardAndMouseDiscoveryStrategy::KeyboardAndMouseDiscoveryStrategy(InputSystem* pInputSystem) :
	VirtualInputDeviceDiscoveryStrategy(pInputSystem)
{
}

KeyboardAndMouseDiscoveryStrategy::~KeyboardAndMouseDiscoveryStrategy()
{
}

void KeyboardAndMouseDiscoveryStrategy::DiscoverVirtualInputDevices()
{
	// FIXME: Since there's no way to know how many keyboards and mouses are connected using Win32, assume there's always 1!
	if (mVirtualInputDevices.size() > 0) return;

	KeyboardAndMouse* pKeyboardAndMouse = WIRE_NEW KeyboardAndMouse(mpInputSystem->GetPlatformKeyMapper());
	mVirtualInputDevices.push_back(pKeyboardAndMouse);

	UpdateListeners();
}

}
