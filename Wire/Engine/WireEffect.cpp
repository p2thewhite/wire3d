// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireEffect.h"

#include "WireGeometry.h"
#include "WireLight.h"
#include "WireMesh.h"
#include "WireRenderer.h"

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
void Effect::Draw(Renderer* pRenderer, Object* pVisible[], UInt min, UInt max,
	Bool restoreState)
{
	// The default drawing function for global effects. Essentially, this
	// draws all the visible leaf geometry, as if no effect was applied.
	// Override to obtain a different behavior.
	for (UInt i = min; i <= max; i++)
	{
		Geometry* pGeometry = DynamicCast<Geometry>(pVisible[i]);
		if (pGeometry)
		{
			pRenderer->Draw(pGeometry, restoreState);
		}
	}
}
