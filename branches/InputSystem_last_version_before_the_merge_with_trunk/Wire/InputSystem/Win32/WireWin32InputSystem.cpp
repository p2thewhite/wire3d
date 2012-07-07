#include "WireWin32InputSystem.h"
#include "WireWin32InputSystemMessageBroker.h"
#include "WireWin32EmulatedWiiMote.h"
#include "WireWin32EmulatedNunchuk.h"
#include "WireMemory.h"
#include <vector>

namespace Wire
{

Win32InputSystem::Win32InputSystem()
{
}

Win32InputSystem::~Win32InputSystem()
{
}

void Win32InputSystem::Capture()
{
	// swap input data buffers
	Win32InputSystemMessageBroker::GetInstance()->SwapBuffers();
	// update the current data buffer on each input device

	for (UInt i = 0; i < mDevices.size(); i++)
	{
		mDevices[i]->SetDataBuffer(Win32InputSystemMessageBroker::GetInstance()->GetCurrentInputDataBuffer());
	}
}

void Win32InputSystem::DoDevicesDiscovery()
{
	// FIXME: since there's no way to know how many keyboards and mouses are connected using Win32 api, 
	// assume there will always be 1 main device (emulated wiimote)
	if (GetMainDevicesCount() == 1)
	{
		return;
	}

	Win32EmulatedWiiMote* pEmulatedWiiMote = WIRE_NEW Win32EmulatedWiiMote();
	AddDevice(pEmulatedWiiMote);
	Win32EmulatedNunchuk* pEmulatedNunchuk = WIRE_NEW Win32EmulatedNunchuk(pEmulatedWiiMote);
	AddDevice(pEmulatedNunchuk);
	pEmulatedWiiMote->SetUp();
	pEmulatedNunchuk->SetUp();
	NotifyDevicesChange();
}

}