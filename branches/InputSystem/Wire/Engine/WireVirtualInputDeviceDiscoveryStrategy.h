#pragma once
#ifndef WIIVIRTUALINPUTDEVICEDISCOVERYSTRATEGY_H_
#define WIIVIRTUALINPUTDEVICEDISCOVERYSTRATEGY_H_

#include "WireTypes.h"
#include "WireVirtualInputDevice.h"
#include "WireInputDeviceDiscoveryListener.h"
#include "WireMemory.h"
#include <vector>
#include <list>

using namespace std;

namespace Wire
{

class InputSystem;

class VirtualInputDeviceDiscoveryStrategy
{
public:
	virtual ~VirtualInputDeviceDiscoveryStrategy();

	virtual void DiscoverVirtualInputDevices() = 0;
	inline const virtual vector<VirtualInputDevice*>& GetFoundVirtualInputDevices() const;
	inline void AddInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener);
	inline void RemoveInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener);
protected:
	InputSystem* mpInputSystem;
	vector<VirtualInputDevice*> mVirtualInputDevices;
	list<InputDeviceDiscoveryListener*> mInputDeviceDiscoveryListeners;

	VirtualInputDeviceDiscoveryStrategy(InputSystem* pInputSystem);
	inline void UpdateListeners() const;
};

#include "WireVirtualInputDeviceDiscoveryStrategy.inl"

}

#endif
