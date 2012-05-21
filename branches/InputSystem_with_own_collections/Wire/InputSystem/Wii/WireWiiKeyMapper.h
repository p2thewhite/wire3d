#ifndef WIREWIIPLATFORMKEYMAPPER_H_
#define WIREWIIPLATFORMKEYMAPPER_H_

#include "WirePlatformKeyMapper.h"

#define WPAD_INVALID_BUTTON	(0x8000<<16)

namespace Wire
{

class WiiKeyMapper: public PlatformKeyMapper
{
public:
	WiiKeyMapper();
	virtual ~WiiKeyMapper();

	virtual UInt GetPlatformKeyForButton(Button button) const;
	virtual UInt GetPlatformKeyForIRAxis(IRAxis axis) const;
	virtual UInt GetPlatformKeyForDigitalAxis(DigitalAxis axis) const;
	virtual UInt GetPlatformKeyForAnalogAxis(AnalogAxis axis) const;
	virtual Button GetButtonForPlatformKey(UInt key) const;
	virtual IRAxis GetIRAxisForPlatformKey(UInt key) const;
	virtual DigitalAxis GetDigitalAxisForPlatformKey(UInt key) const;
	virtual AnalogAxis GetAnalogAxisForPlatformKey(UInt key) const;
};

}

#endif
