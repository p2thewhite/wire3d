#include "WireWin32InputSystem.h"
#include "WireKeyboardAndMouseDiscoveryStrategy.h"
#include "WireWin32KeyMapper.h"
#include "WireInputSystemMessageBroker.h"
#include "WireInputStateBuffer.h"
#include "WireKeyboardAndMouse.h"

namespace Wire
{

Win32InputSystem::Win32InputSystem()
{
	mpDevicesDiscoveryStrategy = WIRE_NEW KeyboardAndMouseDiscoveryStrategy(this);
	mpPlatformKeyMapper = WIRE_NEW Win32KeyMapper();
}

Win32InputSystem::~Win32InputSystem()
{
	WIRE_DELETE mpDevicesDiscoveryStrategy;
	WIRE_DELETE mpPlatformKeyMapper;
}

void Win32InputSystem::Capture()
{
	const InputStateBuffer* pCurrentInputStateBuffer = InputSystemMessageBroker::GetInstance()->GetCurrentInputStateBufferAndSwap();

	//const TArray<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();
	const vector<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();

	//for (UInt i = 0; i < virtualInputDevices.GetQuantity(); i++)
	for (UInt i = 0; i < virtualInputDevices.size(); i++)
	{
		KeyboardAndMouse* pKeyboardAndMouse = static_cast<KeyboardAndMouse*>(virtualInputDevices[i]);
		pKeyboardAndMouse->SetCurrentInputStateBuffer(pCurrentInputStateBuffer);
	}
}

}
