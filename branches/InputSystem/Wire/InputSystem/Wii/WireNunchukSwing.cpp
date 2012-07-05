#include "WireNunchukSwing.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(NunchukSwing, Swing);

NunchukSwing::NunchukSwing(const InputDevice* pParent)
	: Swing(pParent)
{
}

NunchukSwing::~NunchukSwing()
{
}

Float NunchukSwing::GetLeft() const
{
	return 0;
}

Float NunchukSwing::GetRight() const
{
	return 0;
}

Float NunchukSwing::GetUp() const
{
	return 0;
}

Float NunchukSwing::GetDown() const
{
	return 0;
}

Float NunchukSwing::GetForward() const
{
	return 0;
}

Float NunchukSwing::GetBackward() const
{
	return 0;
}

}