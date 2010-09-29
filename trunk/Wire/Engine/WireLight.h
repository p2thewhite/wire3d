#pragma once
#ifndef WIRELIGHT_H
#define WIRELIGHT_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Light : public Object
{
	WIRE_DECLARE_RTTI;

public:
	Light();
	~Light();

};

typedef Pointer<Light> LightPtr;

}

#endif
