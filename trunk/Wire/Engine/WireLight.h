#pragma once
#ifndef WIRELIGHT_H
#define WIRELIGHT_H

#include "WireColorRGB.h"
#include "WireObject.h"
#include "WireVector3.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Light : public Object
{
	WIRE_DECLARE_RTTI;

public:
	enum LightType
	{
		LT_DIRECTIONAL,
		LT_POINT,
		LT_SPOT,
		LT_QUANTITY
	};

	Light(LightType type = LT_DIRECTIONAL);
	virtual ~Light();

	LightType Type;

	ColorRGB Ambient;	// default: BLACK
	ColorRGB Color;		// default: WHITE

	// position and direction in world coordinates
	Vector3F Position;	// default: ZERO
	Vector3F Direction;	// default: -UNIT_Z

	// parameters for spot lights
	Float Angle;		// default: Math::PI
	Float Exponent;		// default: 1

	Bool Enabled;		// default: true
};

typedef Pointer<Light> LightPtr;

}

#endif
