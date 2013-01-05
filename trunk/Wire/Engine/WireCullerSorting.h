// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRECULLERSORTER_H
#define WIRECULLERSORTER_H

#include "WireCuller.h"

namespace Wire
{

class CullerSorting : public Culler
{

public:
	CullerSorting(const Camera* pCamera = NULL,
		UInt maxQuantity = VisibleSet::VS_DEFAULT_MAX_QUANTITY,
		UInt growBy = VisibleSet::VS_DEFAULT_GROWBY);
	virtual ~CullerSorting();

	virtual void ComputeVisibleSet(Spatial* pScene);
	virtual void Insert(Object* pObject);

protected:
	void UnwrapEffectStackAndSort(VisibleSet* pSource, VisibleSet*
		pDestination);
	void QuickSort(TArray<UInt>& pKeys, Object** const pVisible, Int left,
//	void QuickSort(UInt* const pKeys, Object** const pVisible, Int left,
		Int right);
	UInt GetKey(Spatial* pSpatial);

	VisibleSet* mpOpaqueGeometry;
	VisibleSet* mpTransparentGeometry;
	TArray<UInt> mKeys;
};

}

#endif
