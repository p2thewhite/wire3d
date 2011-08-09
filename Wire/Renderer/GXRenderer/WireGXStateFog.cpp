// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireGXRendererData.h"

using namespace Wire;

const UChar PdrRendererData::FOG_DENSITY[StateFog::DF_QUANTITY] =
{
	GX_FOG_PERSP_LIN,	// StateFog::DF_LINEAR
	GX_FOG_ORTHO_EXP,   // StateFog::DF_EXP
	GX_FOG_ORTHO_EXP2   // StateFog::DF_EXPSQR
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateFog* pState)
{
	mspStates[State::FOG] = pState;

	if (pState->Enabled)
	{
 		GXColor color = { pState->Color.R() * 255.0F, pState->Color.G() *
			255.0F, pState->Color.B() * 255.0F, 255};
		GXSetFog(PdrRendererData::FOG_DENSITY[pState->DensityFunc],
			pState->Start, pState->End, mpCamera->GetDMin(), mpCamera->
			GetDMax(), color);
	}
	else
	{
		const GXColor black = { 0, 0, 0, 255};
		GXSetFog(GX_FOG_NONE, 0.0F, 0.0F, 0.0F, 0.0F, black);
	}
}
