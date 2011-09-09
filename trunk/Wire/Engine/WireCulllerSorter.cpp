// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireCullerSorter.h"

#include "WireEffect.h"
#include "WireGeometry.h"
#include "WireLight.h"
#include "WireMaterial.h"
#include "WireStateAlpha.h"

using namespace Wire;

//----------------------------------------------------------------------------
CullerSorter::CullerSorter(const Camera* pCamera, UInt maxQuantity,
	UInt growBy)
	:
	Culler(pCamera, maxQuantity, growBy)
{
	VisibleSet visibleSet(maxQuantity, growBy);
	mVisibleSets.Append(visibleSet);
}

//----------------------------------------------------------------------------
CullerSorter::~CullerSorter()
{
}

//----------------------------------------------------------------------------
void CullerSorter::Insert(Spatial* pObject, Effect* pGlobalEffect)
{
	Geometry* pGeometry = DynamicCast<Geometry>(pObject);
	if (!pGeometry)
	{
		GetVisibleSet(0).Insert(pObject, pGlobalEffect);
		GetVisibleSet(1).Insert(pObject, pGlobalEffect);
		return;
	}

	WIRE_ASSERT(pGlobalEffect == NULL);
	StateAlpha* pAlpha = DynamicCast<StateAlpha>(pGeometry->States[State::
		ALPHA]);
	if (pAlpha)
	{
		if (pAlpha->BlendEnabled)
		{
			GetVisibleSet(1).Insert(pObject, pGlobalEffect);
		}
		else
		{
			GetVisibleSet(0).Insert(pObject, pGlobalEffect);
		}
	}
}
