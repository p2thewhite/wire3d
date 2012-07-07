#ifndef WIN32KEYMAPPEDSWING_H_
#define WIN32KEYMAPPEDSWING_H_

#include "WireSwing.h"
#include "WireTypes.h"
#include "WireRtti.h"

namespace Wire
{

class Win32KeyMappedSwing : public Swing
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedSwing(const InputDevice* pParent);
	virtual ~Win32KeyMappedSwing();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};


}

#endif // WIN32KEYMAPPEDSWING_H_
