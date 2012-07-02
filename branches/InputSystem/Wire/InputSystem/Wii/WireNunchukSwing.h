#ifndef NUNCHUKSWING_H_
#define NUNCHUKSWING_H_

#include "WireSwing.h"
#include "WireTypes.h"

namespace Wire
{

class NunchukSwing : public Swing
{
	WIRE_DECLARE_RTTI;

public:
	NunchukSwing();
	virtual ~NunchukSwing();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};


}

#endif // NUNCHUKSWING_H_
