#include "WireNunchukAnalogPad.h"

WIRE_IMPLEMENT_RTTI(Wire, NunchukAnalogPad, AnalogPad);

namespace Wire
{
	
NunchukAnalogPad::NunchukAnalogPad()
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