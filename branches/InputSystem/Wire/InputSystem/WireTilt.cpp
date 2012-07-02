#include "WireTilt.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Tilt, InputCapability);

Tilt::Tilt(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

Tilt::~Tilt()
{
}

}