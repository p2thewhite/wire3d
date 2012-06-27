#ifndef WIREWIN32KEYMAPPER_H_
#define WIREWIN32KEYMAPPER_H_

#include "WirePlatformKeyMapper.h"
#include "WireButton.h"
#include "WireAxis.h"
#include "WireTypes.h"
#include "WireEulerAngle.h"

namespace Wire
{

class Win32KeyMapper : public PlatformKeyMapper
{
public:
	Win32KeyMapper(void);
	virtual ~Win32KeyMapper(void);

	virtual UInt GetPlatformKeyForButton(Button button) const;
	virtual UInt GetPlatformKeyForIRAxis(IRAxis axis) const;
	virtual UInt GetPlatformKeyForIRAxisRotation(EulerAngle eulerAngle) const;
	virtual UInt GetPlatformKeyForDigitalAxis(DigitalAxis axis) const;
	virtual UInt GetPlatformKeyForAnalogAxis(AnalogAxis axis) const;
	virtual Button GetButtonForPlatformKey(UInt key) const;
	virtual IRAxis GetIRAxisForPlatformKey(UInt key) const;
	virtual EulerAngle GetIRAxisRotationForPlatformKey(UInt key) const;
	virtual DigitalAxis GetDigitalAxisForPlatformKey(UInt key) const;
	virtual AnalogAxis GetAnalogAxisForPlatformKey(UInt key) const;
};

}

#endif
