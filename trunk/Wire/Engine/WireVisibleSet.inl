// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt VisibleSet::GetQuantity() const
{
	return mVisible.GetQuantity();
}

//----------------------------------------------------------------------------
inline UInt VisibleSet::GetMaxQuantity() const
{
	return mVisible.GetMaxQuantity();
}

//----------------------------------------------------------------------------
inline void VisibleSet::SetMaxQuantity(UInt maxQuantity)
{
	mVisible.SetMaxQuantity(maxQuantity);
}

//----------------------------------------------------------------------------
inline Object** VisibleSet::GetVisible()
{
	return mVisible.GetArray();
}

//----------------------------------------------------------------------------
inline Object* VisibleSet::GetVisible(UInt i)
{
	WIRE_ASSERT(i < GetQuantity());
	return mVisible[i];
}

//----------------------------------------------------------------------------
inline void VisibleSet::Clear()
{
	mVisible.SetQuantity(0, false);
}

//----------------------------------------------------------------------------
inline void VisibleSet::Insert(Object* pObject)
{
	mVisible.Append(pObject);
}
