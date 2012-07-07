// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

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
