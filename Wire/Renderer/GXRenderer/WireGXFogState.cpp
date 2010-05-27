#include "WireGXRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

UChar PdrRendererData::msFogDensity[FogState::DF_QUANTITY] =
{
	GX_FOG_PERSP_LIN,	// FogState::DF_LINEAR
	GX_FOG_ORTHO_EXP,   // FogState::DF_EXP
	GX_FOG_ORTHO_EXP2   // FogState::DF_EXPSQR
};

//----------------------------------------------------------------------------
void GXRenderer::SetFogState(FogState* pState)
{
	mspStates[GlobalState::FOG] = pState;

	if (pState->Enabled)
	{
 		GXColor color = { pState->Color.R() * 255.0F, pState->Color.G() *
			255.0F, pState->Color.B() * 255.0F, 255};
		GXSetFog(PdrRendererData::msFogDensity[pState->DensityFunc],
			pState->Start, pState->End, mpCamera->GetDMin(), mpCamera->
			GetDMax(), color);
	}
	else
	{
		const GXColor black = { 0, 0, 0, 255};
		GXSetFog(GX_FOG_NONE, 0.0F, 0.0F, 0.0F, 0.0F, black);
	}
}
