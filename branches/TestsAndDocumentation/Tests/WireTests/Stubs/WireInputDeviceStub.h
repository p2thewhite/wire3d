#pragma once
#ifndef WIREINPUTDEVICESTUB_H
#define WIREINPUTDEVICESTUB_H

#include "WireInputDevice.h"

namespace Wire
{

class InputDeviceStub : public InputDevice
{
public:
	InputDeviceStub();
	virtual ~InputDeviceStub();

	virtual void SetUp();

};

}

#endif