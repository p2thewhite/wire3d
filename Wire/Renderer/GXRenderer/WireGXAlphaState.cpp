#include "WireGXRenderer.h"

using namespace Wire;

UChar GXRenderer::msAlphaSrcBlend[AlphaState::SBM_QUANTITY] =
{
	GX_BL_ZERO,          // AlphaState::SBM_ZERO
	GX_BL_ONE,           // AlphaState::SBM_ONE
	GX_BL_DSTCLR,        // AlphaState::SBM_DST_COLOR
	GX_BL_INVDSTCLR,     // AlphaState::SBM_ONE_MINUS_DST_COLOR
	GX_BL_SRCALPHA,      // AlphaState::SBM_SRC_ALPHA
	GX_BL_INVSRCALPHA,   // AlphaState::SBM_ONE_MINUS_SRC_ALPHA
	GX_BL_DSTALPHA,      // AlphaState::SBM_DST_ALPHA
	GX_BL_INVDSTALPHA,   // AlphaState::SBM_ONE_MINUS_DST_ALPHA
};

UChar GXRenderer::msAlphaDstBlend[AlphaState::DBM_QUANTITY] =
{
	GX_BL_ZERO,          // AlphaState::DBM_ZERO
	GX_BL_ONE,           // AlphaState::DBM_ONE
	GX_BL_SRCCLR,        // AlphaState::DBM_SRC_COLOR
	GX_BL_INVSRCCLR,     // AlphaState::DBM_ONE_MINUS_SRC_COLOR
	GX_BL_SRCALPHA,      // AlphaState::DBM_SRC_ALPHA
	GX_BL_INVSRCALPHA,   // AlphaState::DBM_ONE_MINUS_SRC_ALPHA
	GX_BL_DSTALPHA,      // AlphaState::DBM_DST_ALPHA
	GX_BL_INVDSTALPHA,   // AlphaState::DBM_ONE_MINUS_DST_ALPHA
};

//----------------------------------------------------------------------------
void GXRenderer::SetAlphaState(AlphaState* pState)
{
	mspStates[GlobalState::ALPHA] = pState;

	if (pState->BlendEnabled)
	{
		GXSetBlendMode(GX_BM_BLEND, msAlphaSrcBlend[pState->SrcBlend],
			msAlphaDstBlend[pState->DstBlend], GX_LO_SET);
		GXSetColorUpdate(GX_TRUE);
//		GXSetAlphaUpdate(GX_TRUE);
	}
	else
	{
		GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_SET);
//		GXSetAlphaUpdate(GX_FALSE);
	}
}
