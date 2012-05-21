#ifndef WIREWIIVIRTUALINPUTDISCOVERYSTRATEGY_H_
#define WIREWIIVIRTUALINPUTDISCOVERYSTRATEGY_H_

#include "WireVirtualInputDeviceDiscoveryStrategy.h"
#include "WireWiiController.h"

using namespace std;

namespace Wire
{

class WiiControllerDiscoveryStrategy: public VirtualInputDeviceDiscoveryStrategy
{
public:
	WiiControllerDiscoveryStrategy(InputSystem* pInputSystem);
	virtual ~WiiControllerDiscoveryStrategy();

	virtual void DiscoverVirtualInputDevices();
private:
	static const Int FIRST_CHANNEL;
	static const Int LAST_CHANNEL;

	Int mLastDiscoveredChannel;
	Bool mChanged;

	void NotifyListenersIfChanged();
	void UpdateCurrentlyConnectedChannels();
	void ProbePreviouslyConnectedChannels();
	void DiscoverExtensions(Int channel, WiiController* pWiiController);
	Bool IsWiiMoteConnectedToChannel(Int channel);
};

}

#endif
