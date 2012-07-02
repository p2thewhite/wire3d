#ifndef SHAKE_H_
#define SHAKE_H_

#include "WireInputDevice.h"
#include "WireInputCapability.h"
#include "WireTypes.h"

namespace Wire
{

class Shake : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	Shake(const InputDevice* pParent);
	virtual ~Shake();

	virtual Float GetX() const = 0;
	virtual Float GetY() const = 0;
	virtual Float GetZ() const = 0;

};

}

#endif // SHAKE_H_
