#ifndef NUNCHUKSHAKE_H_
#define NUNCHUKSHAKE_H_

#include "WireShake.h"
#include "WireTypes.h"

namespace Wire
{

class NunchukShake : public Shake
{
	WIRE_DECLARE_RTTI;

public:
	NunchukShake();
	virtual ~NunchukShake();

	virtual Float GetX() const;
	virtual Float GetY() const;
	virtual Float GetZ() const;

};

}

#endif // NUNCHUKSHAKE_H_
