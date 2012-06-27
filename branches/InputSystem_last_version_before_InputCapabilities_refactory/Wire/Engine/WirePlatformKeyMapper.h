#pragma once
#ifndef WIREPLATFORMKEYMAPPER_H_
#define WIREPLATFORMKEYMAPPER_H_

#include "WireButton.h"
#include "WireAxis.h"
#include "WireTypes.h"
#include "WireEulerAngle.h"

namespace Wire
{

class PlatformKeyMapper
{
public:
	PlatformKeyMapper();
	virtual ~PlatformKeyMapper();

	virtual UInt GetPlatformKeyForButton(Button button) const = 0;
	virtual UInt GetPlatformKeyForIRAxis(IRAxis axis) const = 0;
	virtual UInt GetPlatformKeyForIRAxisRotation(EulerAngle eulerAngle) const = 0;
	virtual UInt GetPlatformKeyForDigitalAxis(DigitalAxis axis) const = 0;
	virtual UInt GetPlatformKeyForAnalogAxis(AnalogAxis axis) const = 0;
	virtual Button GetButtonForPlatformKey(UInt key) const = 0;
	virtual IRAxis GetIRAxisForPlatformKey(UInt key) const = 0;
	virtual EulerAngle GetIRAxisRotationForPlatformKey(UInt key) const = 0;
	virtual DigitalAxis GetDigitalAxisForPlatformKey(UInt key) const = 0;
	virtual AnalogAxis GetAnalogAxisForPlatformKey(UInt key) const = 0;
};

}

#endif
