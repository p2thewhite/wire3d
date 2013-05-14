// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
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

class RenderObject;

class CullerSorting : public Culler
{

public:
	CullerSorting(const Camera* pCamera = NULL,
		UInt maxQuantity = VisibleSet::VS_DEFAULT_MAX_QUANTITY,
		UInt growBy = VisibleSet::VS_DEFAULT_GROWBY);
	virtual ~CullerSorting();

	virtual void ComputeVisibleSet(Spatial* pScene);
	virtual void Insert(Object* pObject, Transformation* pTransformation,
		const Vector3F& rPosition = Vector3F::ZERO);

	virtual void Clear();
	virtual void SetMaxQuantity(UInt maxQuantity);

	void Sort();

	static void QuickSort(TPODArray<UInt>& rKeys, Object** const pVisible,
		//	static void QuickSort(UInt* const pKeys, Object** const pVisible,
		Transformation** const pTrafo, Int left, Int right);

protected:
	void UnwrapEffectStackAndSort(VisibleSet* pSource, VisibleSet*
		pDestination, TPODArray<Vector3F>& rPositions);
	UInt GetKey(RenderObject* pRenderObject, const Vector3F& rPosition);

	VisibleSet* mpOpaqueObjects;
	VisibleSet* mpTransparentObjects;
	TPODArray<UInt> mKeys;
	TPODArray<Vector3F> mOpaquePositions;
	TPODArray<Vector3F> mTransparentPositions;
};

}

#endif
