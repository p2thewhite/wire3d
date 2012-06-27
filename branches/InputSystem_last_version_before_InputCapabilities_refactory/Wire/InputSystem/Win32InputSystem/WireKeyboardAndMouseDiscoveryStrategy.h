#ifndef WIREKEYBOARDANDMOUSEDISCOVERYSTRATEGY_H_
#define WIREKEYBOARDANDMOUSEDISCOVERYSTRATEGY_H_

#include "WireVirtualInputDeviceDiscoveryStrategy.h"

namespace Wire
{

class KeyboardAndMouseDiscoveryStrategy: public VirtualInputDeviceDiscoveryStrategy
{
public:
	KeyboardAndMouseDiscoveryStrategy(InputSystem* pInputSystem);
	virtual ~KeyboardAndMouseDiscoveryStrategy();

	virtual void DiscoverVirtualInputDevices();
};

}

#endif
