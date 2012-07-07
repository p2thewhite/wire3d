#ifndef WIIMOTEIR_H_
#define WIIMOTEIR_H_

#include "WireIR.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiMoteIR : public IR
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteIR(const InputDevice* pParent);
	virtual ~WiiMoteIR();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

private:
	Bool IsValid(const WPADData* pData) const;

};


}

#endif // WIIMOTEIR_H_
