#include "WireNunchukShake.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(NunchukShake, Shake);

NunchukShake::NunchukShake(const InputDevice* pParent)
	: Shake(pParent)
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