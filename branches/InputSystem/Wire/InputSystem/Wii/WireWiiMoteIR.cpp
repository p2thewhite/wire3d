#include "WireWiiMoteIR.h"

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteIR, IR);

namespace Wire
{
	
WiiMoteIR::WiiMoteIR()
{
}

WiiMoteIR::~WiiMoteIR()
{
}

Float WiiMoteIR::GetBackward() const
{
	return 0;
}

Float WiiMoteIR::GetDown() const
{
	return 0;
}

Float WiiMoteIR::GetForward() const
{
	return 0;
}

Float WiiMoteIR::GetLeft() const
{
	return 0;
}

Float WiiMoteIR::GetRight() const
{
	return 0;
}

Float WiiMoteIR::GetUp() const
{
	return 0;
}

}