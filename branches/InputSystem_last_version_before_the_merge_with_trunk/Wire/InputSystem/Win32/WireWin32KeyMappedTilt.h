#ifndef WIN32KEYMAPPEDTILT_H_
#define WIN32KEYMAPPEDTILT_H_

#include "WireTilt.h"
#include "WireTypes.h"
#include "WireRtti.h"

namespace Wire
{

class Win32KeyMappedTilt : public Tilt
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedTilt(const InputDevice* pParent);
	virtual ~Win32KeyMappedTilt();

	virtual Float GetBackward() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;

};

}

#endif // WIN32KEYMAPPEDTILT_H_
