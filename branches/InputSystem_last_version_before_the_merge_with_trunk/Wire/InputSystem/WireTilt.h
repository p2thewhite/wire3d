#ifndef TILT_H_
#define TILT_H_

#include "WireInputDevice.h"
#include "WireInputCapability.h"
#include "WireTypes.h"

namespace Wire
{

class Tilt : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	Tilt(const InputDevice* pParent);
	virtual ~Tilt();

	virtual Float GetBackward() const = 0;
	virtual Float GetForward() const = 0;
	virtual Float GetLeft() const = 0;
	virtual Float GetRight() const = 0;

};

}

#endif // TILT_H_
