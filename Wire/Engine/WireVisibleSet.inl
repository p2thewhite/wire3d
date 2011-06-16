// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void VisibleSet::Clear()
{
	mVisible.SetQuantity(0, false);
}

//----------------------------------------------------------------------------
inline UInt VisibleSet::GetQuantity() const
{
	return mVisible.GetQuantity();
}

//----------------------------------------------------------------------------
inline VisibleObject* VisibleSet::GetVisible()
{
	return mVisible.GetArray();
}

//----------------------------------------------------------------------------
inline VisibleObject& VisibleSet::GetVisible(UInt i)
{
	WIRE_ASSERT(i < GetQuantity());
	return mVisible[i];
}
