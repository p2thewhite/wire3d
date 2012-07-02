#ifndef NUNCHUKBUTTONS_H_
#define NUNCHUKBUTTONS_H_

#include "WireButtons.h"
#include "WireTypes.h"

namespace Wire
{

class NunchukButtons : public Buttons
{
	WIRE_DECLARE_RTTI;

public:
	NunchukButtons();
	virtual ~NunchukButtons();

	virtual Bool GetButton(Button button) const;

};

}

#endif // NUNCHUKBUTTONS_H_
