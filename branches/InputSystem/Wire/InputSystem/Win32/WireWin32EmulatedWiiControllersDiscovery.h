#ifndef WIN32EMULATEDWIICONTROLLERSDISCOVERY_H_
#define WIN32EMULATEDWIICONTROLLERSDISCOVERY_H_

#include "WireInputDeviceDiscoveryStrategy.h"

namespace Wire
{

class Win32EmulatedWiiControllersDiscovery : public InputDeviceDiscoveryStrategy
{

public:
	Win32EmulatedWiiControllersDiscovery();
	virtual ~Win32EmulatedWiiControllersDiscovery();

	virtual void Discover();

};

}

#endif // WIN32EMULATEDWIICONTROLLERSDISCOVERY_H_
