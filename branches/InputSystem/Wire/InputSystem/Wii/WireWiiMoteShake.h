#ifndef WIIMOTESHAKE_H_
#define WIIMOTESHAKE_H_

#include "WireShake.h"
#include "WireTypes.h"

namespace Wire
{

class WiiMoteShake : public Shake
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteShake();
	virtual ~WiiMoteShake();

	virtual Float GetX() const;
	virtual Float GetY() const;
	virtual Float GetZ() const;

};

}

#endif // WIIMOTESHAKE_H_
