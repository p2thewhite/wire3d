// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREVISIBLESET_H
#define WIREVISIBLESET_H

#include "WireObject.h"
#include "WireTArray.h"
#include "WireTransformation.h"

namespace Wire
{

class VisibleSet
{

public:
	enum
	{
		VS_DEFAULT_MAX_QUANTITY = 32,
		VS_DEFAULT_GROWBY = 32,
	};

	VisibleSet(UInt maxQuantity = VS_DEFAULT_MAX_QUANTITY, UInt growBy =
		VS_DEFAULT_GROWBY);
	virtual ~VisibleSet ();

	inline UInt GetQuantity() const;
	inline UInt GetMaxQuantity() const;
	inline void SetMaxQuantity(UInt maxQuantity);

	inline void GetSet(Object**& rObjectArrayPtr, Transformation**&
		rTransformationPtr);
	inline void GetElement(UInt i, Object*& rObjectPtr, Transformation*&
		rTransformationPtr);

	inline void Insert(Object* pObject, Transformation* pTransformation);
	inline void Clear();

private:
	TArray<Object*> mVisible;
	TArray<Transformation*> mTransformations;
};

#include "WireVisibleSet.inl"

}

#endif
