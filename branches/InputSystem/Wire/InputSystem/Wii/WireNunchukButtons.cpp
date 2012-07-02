#include "WireNunchukButtons.h"

WIRE_IMPLEMENT_RTTI(Wire, NunchukButtons, Buttons);

namespace Wire
{

NunchukButtons::NunchukButtons()
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