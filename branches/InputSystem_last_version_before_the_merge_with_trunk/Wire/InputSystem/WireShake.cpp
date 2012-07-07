#include "WireShake.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Shake, InputCapability);

Shake::Shake(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

Shake::~Shake()
{
}

}