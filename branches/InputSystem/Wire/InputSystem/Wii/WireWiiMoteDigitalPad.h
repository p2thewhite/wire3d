#ifndef WIIMOTEDIGITALPAD_H_
#define WIIMOTEDIGITALPAD_H_

#include "WireDigitalPad.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiMoteDigitalPad : public DigitalPad
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteDigitalPad(const InputDevice* pParent);
	virtual ~WiiMoteDigitalPad();

	virtual Bool GetDown() const;
	virtual Bool GetLeft() const;
	virtual Bool GetRight() const;
	virtual Bool GetUp() const;

};


}

#endif // WIIMOTEDIGITALPAD_H_
