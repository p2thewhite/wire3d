#include "WireWiiMoteDigitalPad.h"

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteDigitalPad, DigitalPad);

namespace Wire
{

WiiMoteDigitalPad::WiiMoteDigitalPad()
{
}

WiiMoteDigitalPad::~WiiMoteDigitalPad()
{
}

Bool WiiMoteDigitalPad::GetDown() const
{
	return false;
}

Bool WiiMoteDigitalPad::GetLeft() const
{
	return false;
}

Bool WiiMoteDigitalPad::GetRight() const
{
	return false;
}

Bool WiiMoteDigitalPad::GetUp() const
{
	return false;
}

}