#ifndef WIIMOTEDIGITALPAD_H_
#define WIIMOTEDIGITALPAD_H_

#include "WireDigitalPad.h"
#include "WireTypes.h"

namespace Wire
{

class WiiMoteDigitalPad : public DigitalPad
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteDigitalPad();
	virtual ~WiiMoteDigitalPad();

	virtual Bool GetDown() const;
	virtual Bool GetLeft() const;
	virtual Bool GetRight() const;
	virtual Bool GetUp() const;

};


}

#endif // WIIMOTEDIGITALPAD_H_
