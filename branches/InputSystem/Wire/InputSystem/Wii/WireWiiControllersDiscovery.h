#ifndef WIICONTROLLERSDISCOVERY_H_
#define WIICONTROLLERSDISCOVERY_H_

#include "WireInputDeviceDiscoveryStrategy.h"

namespace Wire
{

class WiiControllersDiscovery : public InputDeviceDiscoveryStrategy
{

public:
	WiiControllersDiscovery();
	virtual ~WiiControllersDiscovery();

	virtual void Discover();

};

}

#endif // WIICONTROLLERSDISCOVERY_H_
