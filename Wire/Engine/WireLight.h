// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRELIGHT_H
#define WIRELIGHT_H

#include "WireColorRGB.h"
#include "WireObject.h"
#include "WireTInstanceID.h"
#include "WireVector3.h"

namespace Wire
{

class Light : public Object
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

	TInstanceID<Light> ID;

	LightType Type;

	ColorRGB Ambient;	// default: BLACK
	ColorRGB Color;		// default: WHITE

	// position and direction in world coordinates
	Vector3F Position;	// default: ZERO
	Vector3F Direction;	// default: -UNIT_Z

	// parameters for spot lights
	Float Angle;		// default: Math::PI
	Float Exponent;		// default: 1

	UInt Mask;			// default: 0xFFFFFFFF

	Bool Enabled;		// default: true
};

typedef Pointer<Light> LightPtr;

}

#endif
