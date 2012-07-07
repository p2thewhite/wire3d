#ifndef DIGITALPAD_H_
#define DIGITALPAD_H_

#include "WireInputDevice.h"
#include "WireInputCapability.h"
#include "WireTypes.h"

namespace Wire
{

class DigitalPad : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	DigitalPad(const InputDevice* pParent);
	virtual ~DigitalPad();

	virtual Bool GetDown() const = 0;
	virtual Bool GetLeft() const = 0;
	virtual Bool GetRight() const = 0;
	virtual Bool GetUp() const = 0;

};

}

#endif // DIGITALPAD_H_
