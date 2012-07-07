#include "WireDigitalPad.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(DigitalPad, InputCapability);

DigitalPad::DigitalPad(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

DigitalPad::~DigitalPad()
{
}

}