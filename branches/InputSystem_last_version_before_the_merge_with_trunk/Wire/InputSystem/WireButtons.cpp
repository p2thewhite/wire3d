#include "WireButtons.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Buttons, InputCapability);

Buttons::Buttons(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

Buttons::~Buttons()
{
}

}