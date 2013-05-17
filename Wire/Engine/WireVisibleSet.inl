// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
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
	mTransformations.SetMaxQuantity(maxQuantity);
	mKeys.SetMaxQuantity(maxQuantity);
}

//----------------------------------------------------------------------------
inline void VisibleSet::GetSet(Object**& rObjectArrayPtr, Transformation**&
	rTransformationPtr)
{
	rObjectArrayPtr = mVisible.GetArray();
	rTransformationPtr = mTransformations.GetArray();
}

//----------------------------------------------------------------------------
inline void VisibleSet::GetElement(UInt i, Object*& rObjectPtr,
	Transformation*& rTransformationPtr)
{
	WIRE_ASSERT(i < GetQuantity());
	rObjectPtr = mVisible[i];
	rTransformationPtr = mTransformations[i];
}

//----------------------------------------------------------------------------
inline void VisibleSet::Insert(Object* pObject, Transformation*
	pTransformation, UInt key)
{
	mVisible.Append(pObject);
	mTransformations.Append(pTransformation);
	mKeys.Append(key);
}
