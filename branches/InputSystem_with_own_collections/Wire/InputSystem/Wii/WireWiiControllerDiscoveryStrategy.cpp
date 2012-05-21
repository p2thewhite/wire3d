#include "WireWiiControllerDiscoveryStrategy.h"
#include "WireInputSystem.h"
#include "WireTypes.h"
#include "WireMemory.h"
#include "WireSystem.h"
#include "WireNunchuk.h"
#include "WireWiiController.h"
#include <wiiuse/wpad.h>

namespace Wire
{

const Int WiiControllerDiscoveryStrategy::FIRST_CHANNEL = 0;
const Int WiiControllerDiscoveryStrategy::LAST_CHANNEL = 3;

WiiControllerDiscoveryStrategy::WiiControllerDiscoveryStrategy(InputSystem* pInputSystem) :
		VirtualInputDeviceDiscoveryStrategy(pInputSystem),
		mLastDiscoveredChannel(-1),
		mChanged(false)
{
}

WiiControllerDiscoveryStrategy::~WiiControllerDiscoveryStrategy()
{
}

void WiiControllerDiscoveryStrategy::UpdateCurrentlyConnectedChannels()
{
	// Updating currently connected channels.
	for (Int channel = FIRST_CHANNEL; channel <= mLastDiscoveredChannel; channel++)
	{
		WiiController* pWiiController =	static_cast<WiiController*>(mVirtualInputDevices[channel]);

		if (pWiiController->HasWiiExtension())
		{
			continue;
		}

		DiscoverExtensions(channel, pWiiController);
	}
}

void WiiControllerDiscoveryStrategy::ProbePreviouslyConnectedChannels()
{
	// Probing previously disconnected channels.
	for (Int channel = mLastDiscoveredChannel + 1; channel <= LAST_CHANNEL;	channel++)
	{
		if (!IsWiiMoteConnectedToChannel(channel))
		{
			continue;
		}

		WiiController * pWiiController = WIRE_NEW WiiController(channel);
		mVirtualInputDevices.Append(pWiiController);

		pWiiController->SetWiiMote(WIRE_NEW WiiMote(mpInputSystem->GetPlatformKeyMapper()));

		DiscoverExtensions(channel, pWiiController);

		mLastDiscoveredChannel = channel;

		mChanged = true;
	}
}

void WiiControllerDiscoveryStrategy::NotifyListenersIfChanged()
{
	if (mChanged)
	{
		UpdateListeners();
		mChanged = false;
	}
}

void WiiControllerDiscoveryStrategy::DiscoverVirtualInputDevices()
{
	UpdateCurrentlyConnectedChannels();
	ProbePreviouslyConnectedChannels();
	NotifyListenersIfChanged();
}

void WiiControllerDiscoveryStrategy::DiscoverExtensions(Int channel, WiiController* pWiiController)
{
	expansion_t data;
	WPAD_Expansion(channel, &data);

	if (data.type == WPAD_EXP_NONE)
	{
		return;
	}

	switch (data.type)
	{
	case WPAD_EXP_NUNCHUK:
		pWiiController->SetWiiExtension(WIRE_NEW Nunchuk(mpInputSystem->GetPlatformKeyMapper()));
		break;
	default:
		System::Assert("Unknown wii extension found.", "WireWiiControllerDiscoveryStrategy.cpp", 82);
		return;
	}

	mChanged = true;
}

Bool WiiControllerDiscoveryStrategy::IsWiiMoteConnectedToChannel(Int channel)
{
	UInt type;
	return (WPAD_Probe(channel, &type) == WPAD_ERR_NONE);
}

}
