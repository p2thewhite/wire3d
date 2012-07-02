#ifndef WIIMOTEIR_H_
#define WIIMOTEIR_H_

#include "WireIR.h"
#include "WireTypes.h"

namespace Wire
{

class WiiMoteIR : public IR
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteIR();
	virtual ~WiiMoteIR();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};


}

#endif // WIIMOTEIR_H_
