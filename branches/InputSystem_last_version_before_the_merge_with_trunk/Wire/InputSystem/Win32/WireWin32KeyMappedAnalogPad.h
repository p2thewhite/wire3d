#ifndef WIN32KEYMAPPEDANALOGPAD_H_
#define WIN32KEYMAPPEDANALOGPAD_H_

#include "WireAnalogPad.h"
#include "WireTypes.h"
#include "WireRtti.h"

namespace Wire
{

class Win32KeyMappedAnalogPad : public AnalogPad
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedAnalogPad(const InputDevice* pParent);
	virtual ~Win32KeyMappedAnalogPad();

	virtual Float GetDown() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};

}

#endif // WIN32KEYMAPPEDANALOGPAD_H_
