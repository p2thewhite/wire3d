#pragma once
#ifndef WIRELIGHT_H
#define WIRELIGHT_H

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

	Vector3F Position;
	Vector3F Direction;

};

typedef Pointer<Light> LightPtr;

}

#endif
