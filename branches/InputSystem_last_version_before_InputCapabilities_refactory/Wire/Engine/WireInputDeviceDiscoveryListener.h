#pragma once
#ifndef WIREINPUTDEVICEDISCOVERYLISTENER_H_
#define WIREINPUTDEVICEDISCOVERYLISTENER_H_

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
#endif
