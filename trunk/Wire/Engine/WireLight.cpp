#include "WireLight.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Light, Object);

//----------------------------------------------------------------------------
Light::Light()
	:
	Ambient(ColorRGB::BLACK),
	Color(ColorRGB::WHITE),
	Position(Vector3F::ZERO),
	Direction(Vector3F::UNIT_Z)
{
}

//----------------------------------------------------------------------------
Light::~Light()
{
}
