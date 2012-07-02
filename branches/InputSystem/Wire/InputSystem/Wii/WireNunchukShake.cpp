#include "WireNunchukShake.h"

WIRE_IMPLEMENT_RTTI(Wire, NunchukShake, Shake);

namespace Wire
{

NunchukShake::NunchukShake()
{
}

NunchukShake::~NunchukShake()
{
}

Float NunchukShake::GetX() const
{
	return 0;
}

Float NunchukShake::GetY() const
{
	return 0;
}

Float NunchukShake::GetZ() const
{
	return 0;
}

}