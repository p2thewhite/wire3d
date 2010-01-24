#pragma once
#ifndef WIREVISIBLEOBJECT_H
#define WIREVISIBLEOBJECT_H

#include "../Foundation/WireSystem.h"

namespace Wire
{

class Effect;
class Spatial;

class /*WIRE_ENGINE_ITEM*/ VisibleObject
{

public:
	Spatial* Object;
	Effect* GlobalEffect;

	Bool IsDrawable() const;
};

#include "WireVisibleObject.inl"

}

#endif
