#pragma once
#ifndef WIREVISIBLEOBJECT_H
#define WIREVISIBLEOBJECT_H

#include "WireTypes.h"

namespace Wire
{

class Effect;
class Spatial;

class VisibleObject
{

public:
	Spatial* Object;
	Effect* GlobalEffect;

	inline Bool IsDrawable() const;
};

#include "WireVisibleObject.inl"

}

#endif
