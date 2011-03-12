#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

UChar PdrRendererData::sAlphaSrcBlend[StateAlpha::SBM_QUANTITY] =
{
	GX_BL_ZERO,          // StateAlpha::SBM_ZERO
	GX_BL_ONE,           // StateAlpha::SBM_ONE
	GX_BL_DSTCLR,        // StateAlpha::SBM_DST_COLOR
	GX_BL_INVDSTCLR,     // StateAlpha::SBM_ONE_MINUS_DST_COLOR
	GX_BL_SRCALPHA,      // StateAlpha::SBM_SRC_ALPHA
	GX_BL_INVSRCALPHA,   // StateAlpha::SBM_ONE_MINUS_SRC_ALPHA
	GX_BL_DSTALPHA,      // StateAlpha::SBM_DST_ALPHA
	GX_BL_INVDSTALPHA,   // StateAlpha::SBM_ONE_MINUS_DST_ALPHA
};

UChar PdrRendererData::sAlphaDstBlend[StateAlpha::DBM_QUANTITY] =
{
	GX_BL_ZERO,          // StateAlpha::DBM_ZERO
	GX_BL_ONE,           // StateAlpha::DBM_ONE
	GX_BL_SRCCLR,        // StateAlpha::DBM_SRC_COLOR
	GX_BL_INVSRCCLR,     // StateAlpha::DBM_ONE_MINUS_SRC_COLOR
	GX_BL_SRCALPHA,      // StateAlpha::DBM_SRC_ALPHA
	GX_BL_INVSRCALPHA,   // StateAlpha::DBM_ONE_MINUS_SRC_ALPHA
	GX_BL_DSTALPHA,      // StateAlpha::DBM_DST_ALPHA
	GX_BL_INVDSTALPHA,   // StateAlpha::DBM_ONE_MINUS_DST_ALPHA
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateAlpha* pState)
{
	mspStates[State::ALPHA] = pState;

	if (pState->BlendEnabled)
	{
		GXSetBlendMode(GX_BM_BLEND,
			PdrRendererData::sAlphaSrcBlend[pState->SrcBlend],
			PdrRendererData::sAlphaDstBlend[pState->DstBlend], GX_LO_SET);
		GXSetColorUpdate(GX_TRUE);
//		GXSetAlphaUpdate(GX_TRUE);
	}
	else
	{
		GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_SET);
//		GXSetAlphaUpdate(GX_FALSE);
	}
}