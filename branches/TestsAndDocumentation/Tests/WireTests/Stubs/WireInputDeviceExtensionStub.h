#pragma once
#ifndef WIREINPUTDEVICEEXTENSIONSTUB_H
#define WIREINPUTDEVICEEXTENSIONSTUB_H

#include "WireInputDeviceExtension.h"
#include "WireTArray.h"
#include "WireMainInputDeviceStub.h"
#include "WireInputCapabilityStub.h"

namespace Wire
{

class InputDeviceExtensionStub : public InputDeviceExtension
{
	WIRE_DECLARE_RTTI;

public:
	InputDeviceExtensionStub(MainInputDevice* pMainInputDevice);
	virtual ~InputDeviceExtensionStub();

	virtual void SetUp();

	inline const MainInputDevice* GetMainInputDevice_TEST()
	{
		return GetMainInputDevice();
	}

};

}

#endif