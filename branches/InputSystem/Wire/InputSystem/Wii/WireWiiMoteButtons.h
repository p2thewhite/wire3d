#ifndef WIIMOTEBUTTONS_H_
#define WIIMOTEBUTTONS_H_

#include "WireButtons.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiMoteButtons : public Buttons
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteButtons(const InputDevice* pParent);
	virtual ~WiiMoteButtons();

	virtual Bool GetButton(Button button) const;

};

}

#endif // WIIMOTEBUTTONS_H_
