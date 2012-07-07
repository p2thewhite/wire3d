#include "WireWiiMoteShake.h"
#include "WireWiiMoteButtons.h"
#include "WireWiiMote.h"
#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(WiiMoteShake, Shake);

WiiMoteShake::WiiMoteShake(const InputDevice* pParent)
	: Shake(pParent)
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