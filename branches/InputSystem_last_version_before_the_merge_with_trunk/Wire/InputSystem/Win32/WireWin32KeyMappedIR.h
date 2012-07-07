#ifndef WIN32KEYMAPPEDIR_H_
#define WIN32KEYMAPPEDIR_H_

#include "WireIR.h"
#include "WireRtti.h"
#include "WireTypes.h"

namespace Wire
{

class Win32KeyMappedIR : public IR
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedIR(const InputDevice* pParent);
	virtual ~Win32KeyMappedIR();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};

}

#endif // WIN32KEYMAPPEDIR_H_
