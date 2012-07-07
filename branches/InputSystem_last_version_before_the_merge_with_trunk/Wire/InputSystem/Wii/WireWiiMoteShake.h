#ifndef WIIMOTESHAKE_H_
#define WIIMOTESHAKE_H_

#include "WireShake.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiMoteShake : public Shake
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteShake(const InputDevice* pParent);
	virtual ~WiiMoteShake();

	virtual Float GetX() const;
	virtual Float GetY() const;
	virtual Float GetZ() const;

private:
	WPADData* GetDataFromParent() const;

};

}

#endif // WIIMOTESHAKE_H_
