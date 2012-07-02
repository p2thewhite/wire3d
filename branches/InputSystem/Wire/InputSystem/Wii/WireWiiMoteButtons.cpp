#include "WireWiiMoteButtons.h"

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteButtons, Buttons);

namespace Wire
{

WiiMoteButtons::WiiMoteButtons()
{
}

WiiMoteButtons::~WiiMoteButtons()
{
}

Bool WiiMoteButtons::GetButton(Button button) const
{
	return false;
}

}