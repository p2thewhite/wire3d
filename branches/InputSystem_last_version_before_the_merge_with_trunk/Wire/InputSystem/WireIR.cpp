#include "WireIR.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(IR, InputCapability);

IR::IR(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

IR::~IR()
{
}

}