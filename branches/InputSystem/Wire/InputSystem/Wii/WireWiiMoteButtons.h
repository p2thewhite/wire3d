#ifndef WIIMOTEBUTTONS_H_
#define WIIMOTEBUTTONS_H_

#include "WireButtons.h"
#include "WireTypes.h"

namespace Wire
{

class WiiMoteButtons : public Buttons
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteButtons();
	virtual ~WiiMoteButtons();

	virtual Bool GetButton(Button button) const;

};

}

#endif // WIIMOTEBUTTONS_H_
