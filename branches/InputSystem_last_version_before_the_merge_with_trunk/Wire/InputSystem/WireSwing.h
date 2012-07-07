#ifndef SWING_H_
#define SWING_H_

#include "WireInputDevice.h"
#include "WireInputCapability.h"
#include "WireTypes.h"

namespace Wire
{

class Swing : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	Swing(const InputDevice* pParent);
	virtual ~Swing();

	virtual Float GetBackward() const = 0;
	virtual Float GetDown() const = 0;
	virtual Float GetForward() const = 0;
	virtual Float GetLeft() const = 0;
	virtual Float GetRight() const = 0;
	virtual Float GetUp() const = 0;

};

}

#endif // SWING_H_
