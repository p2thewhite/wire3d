#include "WireWiiInputSystem.h"
#include "WireWiiControllerDiscoveryStrategy.h"
#include "WireWiiKeyMapper.h"
#include "WireWiiMote.h"
#include "WireMemory.h"
#include "WireObject.h"
#include <wiiuse/wpad.h>

namespace Wire
{

UInt WiiInputSystem::mEventCounter = 0;

WiiInputSystem::WiiInputSystem(Float screenWidth, Float screenHeight) :
		InputSystem(),
		mScreenWidth(screenWidth),
		mScreenHeight(screenHeight)
{
	mpDevicesDiscoveryStrategy = WIRE_NEW WiiControllerDiscoveryStrategy(this);
	mpPlatformKeyMapper = WIRE_NEW WiiKeyMapper();

	PAD_Init();
	WPAD_Init();
}

WiiInputSystem::~WiiInputSystem()
{
	WPAD_Shutdown();
}

void WiiInputSystem::AfterInputDevicesDiscovery()
{
	const TArray<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();

	for (UInt i = 0; i < virtualInputDevices.GetQuantity(); i++)
	{
		WiiController* pWiiController = dynamic_cast<WiiController*>(virtualInputDevices[i]);
		pWiiController->SetIRCaptureArea(0, 0, mScreenWidth, mScreenHeight);
	}
}

void WiiInputSystem::Capture()
{
	WPAD_ReadPending(WPAD_CHAN_ALL,	&WiiInputSystem::ReadWPADPendingEventsCallback);

	InputSystem::Capture();
}

void WiiInputSystem::ReadWPADPendingEventsCallback(int channel, const WPADData* data)
{
	WiiInputSystem::mEventCounter++;
}

}

