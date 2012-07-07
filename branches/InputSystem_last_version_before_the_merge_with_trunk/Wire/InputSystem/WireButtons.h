#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "WireInputDevice.h"
#include "WireInputCapability.h"
#include "WireButton.h"
#include "WireTypes.h"

namespace Wire
{

class Buttons : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	Buttons(const InputDevice* pParent);
	virtual ~Buttons();

	virtual Bool GetButton(Button button) const = 0;

};

}

#endif // BUTTONS_H_
