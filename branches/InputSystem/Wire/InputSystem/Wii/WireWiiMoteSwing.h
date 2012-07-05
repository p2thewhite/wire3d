#ifndef WIIMOTESWING_H_
#define WIIMOTESWING_H_

#include "WireSwing.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiMoteSwing : public Swing
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteSwing(const InputDevice* pParent);
	virtual ~WiiMoteSwing();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};

}

#endif // WIIMOTESWING_H_
