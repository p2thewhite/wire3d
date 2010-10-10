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
	Light();
	~Light();

	ColorRGB Ambient;	// default: BLACK
	ColorRGB Color;		// default: WHITE

	// position and direction in world coordinates
	Vector3F Position;	// default: ZERO
	Vector3F Direction;	// default: UNIT_Z
};

typedef Pointer<Light> LightPtr;

}

#endif
