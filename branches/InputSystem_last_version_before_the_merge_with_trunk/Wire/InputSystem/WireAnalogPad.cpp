#include "WireAnalogPad.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(AnalogPad, InputCapability);

AnalogPad::AnalogPad(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

AnalogPad::~AnalogPad()
{
}

}