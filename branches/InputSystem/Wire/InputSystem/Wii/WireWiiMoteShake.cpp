#include "WireWiiMoteShake.h"

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteShake, Shake);

namespace Wire
{

WiiMoteShake::WiiMoteShake()
{
}

WiiMoteShake::~WiiMoteShake()
{
}

Float WiiMoteShake::GetX() const
{
	return 0;
}

Float WiiMoteShake::GetY() const
{
	return 0;
}

Float WiiMoteShake::GetZ() const
{
	return 0;
}

}