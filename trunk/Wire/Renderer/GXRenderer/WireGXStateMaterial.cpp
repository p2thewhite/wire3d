// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetState(StateMaterial* pState)
{
	mspStates[State::MATERIAL] = pState;

	GXColor material;
	material.r = static_cast<UChar>(pState->Ambient.R() * 255.0F);
	material.g = static_cast<UChar>(pState->Ambient.G() * 255.0F);
	material.b = static_cast<UChar>(pState->Ambient.B() * 255.0F);
	material.a = static_cast<UChar>(pState->Ambient.A() * 255.0F);

	GXSetChanMatColor(GX_COLOR0A0, material);
}
