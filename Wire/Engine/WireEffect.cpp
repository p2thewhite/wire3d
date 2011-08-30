// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireEffect.h"

#include "WireGeometry.h"
#include "WireLight.h"
#include "WireRenderer.h"
#include "WireVisibleObject.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Effect, Object);

//----------------------------------------------------------------------------
Effect::Effect()
{
}

//----------------------------------------------------------------------------
Effect::~Effect()
{
}

//----------------------------------------------------------------------------
void Effect::Draw(Renderer* pRenderer, Spatial*, UInt min, UInt max,
	VisibleObject* pVisible)
{
	// The default drawing function for global effects. Essentially, this
	// draws all the visible leaf geometry, as if no effect was applied.
	// Override to obtain a different behavior.
	for (UInt i = min; i <= max; i++)
	{
		if (pVisible[i].IsDrawable())
		{
			Geometry* pGeometry = DynamicCast<Geometry>(pVisible[i].Object);
			WIRE_ASSERT(pGeometry);
			pRenderer->Draw(pGeometry);
		}
	}
}
