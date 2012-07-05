#ifndef NUNCHUKANALOGPAD_H_
#define NUNCHUKANALOGPAD_H_

#include "WireAnalogPad.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"

namespace Wire
{

class NunchukAnalogPad : public AnalogPad
{
	WIRE_DECLARE_RTTI;

public:
	NunchukAnalogPad(const InputDevice* pParent);
	virtual ~NunchukAnalogPad();

	virtual Float GetDown() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};


}

#endif // NUNCHUKANALOGPAD_H_
