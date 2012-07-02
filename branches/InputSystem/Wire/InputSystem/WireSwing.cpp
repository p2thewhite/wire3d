#include "WireSwing.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Swing, InputCapability);

Swing::Swing(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

Swing::~Swing()
{
}

}