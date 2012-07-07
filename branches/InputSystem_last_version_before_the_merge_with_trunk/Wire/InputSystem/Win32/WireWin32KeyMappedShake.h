#ifndef WIN32KEYMAPPEDSHAKE_H_
#define WIN32KEYMAPPEDSHAKE_H_

#include "WireShake.h"
#include "WireTypes.h"
#include "WireRtti.h"

namespace Wire
{

class Win32KeyMappedShake : public Shake
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedShake(const InputDevice* pParent);
	virtual ~Win32KeyMappedShake();

	virtual Float GetX() const;
	virtual Float GetY() const;
	virtual Float GetZ() const;

};

}

#endif // WIN32KEYMAPPEDSHAKE_H_
