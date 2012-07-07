#include "WireWiiMoteSwing.h"
#include "WireWiiMoteButtons.h"
#include "WireWiiMote.h"
#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(WiiMoteSwing, Swing);
	
WiiMoteSwing::WiiMoteSwing(const InputDevice* pParent)
	: Swing(pParent)
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