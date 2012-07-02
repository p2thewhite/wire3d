#ifndef INPUTDEVICEDISCOVERYLISTENER_H_
#define INPUTDEVICEDISCOVERYLISTENER_H_

namespace Wire
{

class InputDeviceDiscoveryListener
{

public:
	InputDeviceDiscoveryListener();
	virtual ~InputDeviceDiscoveryListener();

	virtual void Update() = 0;

};

}

#endif // INPUTDEVICEDISCOVERYLISTENER_H_
