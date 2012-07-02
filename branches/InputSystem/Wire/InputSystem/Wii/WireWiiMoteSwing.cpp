#include "WireWiiMoteSwing.h"

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteSwing, Swing);

namespace Wire
{
	
WiiMoteSwing::WiiMoteSwing()
{
}

WiiMoteSwing::~WiiMoteSwing()
{
}

Float WiiMoteSwing::GetBackward() const
{
	return 0;
}

Float WiiMoteSwing::GetDown() const
{
	return 0;
}

Float WiiMoteSwing::GetForward() const
{
	return 0;
}

Float WiiMoteSwing::GetLeft() const
{
	return 0;
}

Float WiiMoteSwing::GetRight() const
{
	return 0;
}

Float WiiMoteSwing::GetUp() const
{
	return 0;
}

}