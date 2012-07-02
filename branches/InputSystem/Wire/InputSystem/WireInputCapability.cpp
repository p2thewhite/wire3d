#include "WireInputCapability.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(InputCapability, Object);

InputCapability::InputCapability(const InputDevice* pParent) 
	: mpParent(pParent)
{
}

InputCapability::~InputCapability()
{
}

const InputDevice* InputCapability::GetParent() const
{
	return mpParent;
}

}