#ifndef IR_H_
#define IR_H_

#include "WireInputDevice.h"
#include "WireInputCapability.h"
#include "WireTypes.h"

namespace Wire
{

class IR : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	IR(const InputDevice* pParent);
	virtual ~IR();

	virtual Float GetBackward() const = 0;
	virtual Float GetDown() const = 0;
	virtual Float GetForward() const = 0;
	virtual Float GetLeft() const = 0;
	virtual Float GetRight() const = 0;
	virtual Float GetUp() const = 0;

};

}

#endif // IR_H_
