#include "WireNunchukTilt.h"

WIRE_IMPLEMENT_RTTI(Wire, NunchukTilt, Tilt);

namespace Wire
{

NunchukTilt::NunchukTilt()
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