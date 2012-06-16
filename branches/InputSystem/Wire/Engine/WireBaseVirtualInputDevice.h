#pragma once
#ifndef WIREBASEVIRTUALINPUTDEVICE_H_
#define WIREBASEVIRTUALINPUTDEVICE_H_

#include "WireVirtualInputDevice.h"
#include "WireTypes.h"
#include "WireButton.h"
#include "WireAxis.h"
#include "WireEulerAngle.h"
#include "WireInputDevice.h"
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

namespace Wire
{

class InputSystem;

class BaseVirtualInputDevice: public VirtualInputDevice
{
public:
	virtual ~BaseVirtualInputDevice();

	virtual void Capture() = 0;
	virtual const char* GetName() const;
	virtual Bool GetButton(Button button) const;
	virtual Float GetIRAxis(IRAxis axis) const;
	virtual Float GetIRAxisRotation(EulerAngle eulerAngle) const;
	virtual Float GetDigitalAxis(DigitalAxis axis) const;
	virtual Float GetAnalogAxis(AnalogAxis axis) const;
	inline virtual Bool HasCapability(InputCapability capability) const;
	inline virtual const set<InputCapability>& GetCapabilities() const;

	friend class InputSystem;
	friend class InputDevice;
protected:
	BaseVirtualInputDevice();

	void AddInputDevice(InputDevice* pInputDevice);
	void AddInputDevice(InputDevice* pInputDevice, UInt index);
	inline InputDevice* GetInputDevice(UInt index);
private:
	vector<InputDevice*> mInputDevices;
	set<InputCapability> mCapabilities;

	void UpdateCapabilities();
};

#include "WireBaseVirtualInputDevice.inl"

}

#endif
