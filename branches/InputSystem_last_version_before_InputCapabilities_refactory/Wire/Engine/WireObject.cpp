// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireObject.h"

using namespace Wire;

const Rtti Object::TYPE("Wire.Object", NULL);

//----------------------------------------------------------------------------
Object::Object()
	:
	mReferences(0)
{
}

//----------------------------------------------------------------------------
Object::~Object ()
{
	// This assert traps the case when someone tries 'delete spObject'
	// for a smart-pointer-based object. Instead, use 'spObject = NULL'.
	WIRE_ASSERT(mReferences == 0);
}

//----------------------------------------------------------------------------
void Object::DecrementReferences()
{
	if (--mReferences == 0)
	{
		WIRE_DELETE this;
	}
}
