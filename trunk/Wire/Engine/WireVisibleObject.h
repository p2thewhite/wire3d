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
#include "WireGeometry.h"
#include "WireMesh.h"

namespace Wire
{

class Object;

class VisibleObject
{

public:
	Object* VObject;
};

#include "WireVisibleObject.inl"

}

#endif
