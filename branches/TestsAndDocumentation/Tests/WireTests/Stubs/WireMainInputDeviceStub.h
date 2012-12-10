#ifndef WIREMAININPUTDEVICESTUB_H
#define WIREMAININPUTDEVICESTUB_H

#include "WireMainInputDevice.h"
#include "WireTArray.h"
#include "WireInputCapabilityStub.h"

namespace Wire
{

class MainInputDeviceStub : public MainInputDevice
{
	WIRE_DECLARE_RTTI;

public:
	MainInputDeviceStub();
	virtual ~MainInputDeviceStub();

	virtual void SetUp();

};

}

#endif