#include "WireWin32InputSystem.h"
#include "WireWin32InputSystemMessageBroker.h"
#include "WireWin32EmulatedWiiControllersDiscovery.h"
#include "WireMemory.h"
#include <vector>

namespace Wire
{

Win32InputSystem::Win32InputSystem()
{
	mpInputDeviceDiscoveryStrategy = WIRE_NEW Win32EmulatedWiiControllersDiscovery();
}

Win32InputSystem::~Win32InputSystem()
{
	WIRE_DELETE mpInputDeviceDiscoveryStrategy;
}

void Win32InputSystem::Capture()
{
	// swap input data buffers
	Win32InputSystemMessageBroker::GetInstance()->SwapBuffers();
	// update the current data buffer on each input device
	const std::vector<InputDevice*>& inputDevices = mpInputDeviceDiscoveryStrategy->GetInputDevices();
	
	for (UInt i = 0; i < inputDevices.size(); i++)
	{
		inputDevices[i]->SetDataBuffer(Win32InputSystemMessageBroker::GetInstance()->GetCurrentInputDataBuffer());
	}
}

}