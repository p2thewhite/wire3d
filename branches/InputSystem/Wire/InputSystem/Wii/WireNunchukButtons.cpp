#include "WireNunchukButtons.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(NunchukButtons, Buttons);

NunchukButtons::NunchukButtons(const InputDevice* pParent)
	: Buttons(pParent)
{
}

NunchukButtons::~NunchukButtons()
{
}

Bool NunchukButtons::GetButton(Button button) const
{
	return false;
}

}