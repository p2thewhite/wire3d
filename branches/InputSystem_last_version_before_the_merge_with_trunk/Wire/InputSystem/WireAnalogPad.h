#ifndef ANALOGPAD_H_
#define ANALOGPAD_H_

#include "WireInputDevice.h"
#include "WireInputCapability.h"
#include "WireTypes.h"

namespace Wire
{

class AnalogPad : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	AnalogPad(const InputDevice* pParent);
	virtual	~AnalogPad();

	virtual Float GetDown() const = 0;
	virtual Float GetLeft() const = 0;
	virtual Float GetRight() const = 0;
	virtual Float GetUp() const = 0;

};

}

#endif // ANALOGPAD_H_
