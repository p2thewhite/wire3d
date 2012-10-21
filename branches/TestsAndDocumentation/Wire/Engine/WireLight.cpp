// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireLight.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Light, Object);

//----------------------------------------------------------------------------
Light::Light(LightType type)
	:
	Type(type),
	Ambient(ColorRGB::BLACK),
	Color(ColorRGB::WHITE),
	Position(Vector3F::ZERO),
	Direction(-Vector3F::UNIT_Z),
	Angle(MathF::PI),
	Exponent(1.0F),
	Enabled(true)
{
}

//----------------------------------------------------------------------------
Light::~Light()
{
}
