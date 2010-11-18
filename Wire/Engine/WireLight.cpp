#include "WireLight.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Light, Object);

//----------------------------------------------------------------------------
Light::Light(LightType type)
	:
	Type(type),
	Ambient(ColorRGB::BLACK),
	Color(ColorRGB::WHITE),
	Position(Vector3F::ZERO),
	Direction(-Vector3F::UNIT_Z),
	Angle(MathF::PI),
	Exponent(1.0F)
{
}

//----------------------------------------------------------------------------
Light::~Light()
{
}
