// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireVisibleSet.h"

using namespace Wire;

//----------------------------------------------------------------------------
VisibleSet::VisibleSet(UInt maxQuantity, UInt growBy)
	:
	mVisible(maxQuantity, growBy)
{
}

//----------------------------------------------------------------------------
VisibleSet::~VisibleSet()
{
}

//----------------------------------------------------------------------------
void VisibleSet::Insert(Object* pObject)
{
	VisibleObject visible;
	visible.VObject = pObject;
	mVisible.Append(visible);
}
