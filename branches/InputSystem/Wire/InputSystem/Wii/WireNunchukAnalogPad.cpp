#include "WireNunchukAnalogPad.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(NunchukAnalogPad, AnalogPad);
	
NunchukAnalogPad::NunchukAnalogPad(const InputDevice* pParent)
	: AnalogPad(pParent)
{
}

NunchukAnalogPad::~NunchukAnalogPad()
{
}

Float NunchukAnalogPad::GetDown() const
{
	return 0;
}

Float NunchukAnalogPad::GetLeft() const
{
	return 0;
}

Float NunchukAnalogPad::GetRight() const
{
	return 0;
}

Float NunchukAnalogPad::GetUp() const
{
	return 0;
}

}