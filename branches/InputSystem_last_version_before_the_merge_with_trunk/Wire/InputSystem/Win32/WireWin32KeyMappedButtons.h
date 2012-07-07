#ifndef WIN32KEYMAPPEDBUTTONS_H_
#define WIN32KEYMAPPEDBUTTONS_H_

#include "WireButtons.h"
#include "WireRtti.h"
#include "WireTypes.h"

namespace Wire
{

class Win32KeyMappedButtons : public Buttons
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedButtons(const InputDevice* pParent);
	virtual ~Win32KeyMappedButtons();

	virtual Bool GetButton(Button button) const;

};

}

#endif // WIN32KEYMAPPEDBUTTONS_H_
