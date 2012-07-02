#ifndef WIN32KEYMAPPEDDIGITALPAD_H_
#define WIN32KEYMAPPEDDIGITALPAD_H_

#include "WireDigitalPad.h"
#include "WireTypes.h"
#include "WireRtti.h"

namespace Wire
{

class Win32KeyMappedDigitalPad : public DigitalPad
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedDigitalPad(const InputDevice* pParent);
	virtual ~Win32KeyMappedDigitalPad();

	virtual Bool GetDown() const;
	virtual Bool GetLeft() const;
	virtual Bool GetRight() const;
	virtual Bool GetUp() const;

};

}

#endif // WIN32KEYMAPPEDDIGITALPAD_H_
