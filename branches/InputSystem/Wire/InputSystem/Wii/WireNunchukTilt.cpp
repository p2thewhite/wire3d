#include "WireNunchukTilt.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(NunchukTilt, Tilt);

NunchukTilt::NunchukTilt(const InputDevice* pParent)
	: Tilt(pParent)
{
}

NunchukTilt::~NunchukTilt()
{
}

Float NunchukTilt::GetBackward() const
{
	return 0;
}

Float NunchukTilt::GetForward() const
{
	return 0;
}

Float NunchukTilt::GetLeft() const
{
	return 0;
}

Float NunchukTilt::GetRight() const
{
	return 0;
}

}