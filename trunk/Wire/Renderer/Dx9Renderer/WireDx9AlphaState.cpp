#include "WireDx9Renderer.h"

using namespace Wire;

DWORD Dx9Renderer::msAlphaSrcBlend[AlphaState::SBM_QUANTITY] =
{
	D3DBLEND_ZERO,          // AlphaState::SBM_ZERO
	D3DBLEND_ONE,           // AlphaState::SBM_ONE
	D3DBLEND_DESTCOLOR,     // AlphaState::SBM_DST_COLOR
	D3DBLEND_INVDESTCOLOR,  // AlphaState::SBM_ONE_MINUS_DST_COLOR
	D3DBLEND_SRCALPHA,      // AlphaState::SBM_SRC_ALPHA
	D3DBLEND_INVSRCALPHA,   // AlphaState::SBM_ONE_MINUS_SRC_ALPHA
	D3DBLEND_DESTALPHA,     // AlphaState::SBM_DST_ALPHA
	D3DBLEND_INVDESTALPHA,  // AlphaState::SBM_ONE_MINUS_DST_ALPHA
};

DWORD Dx9Renderer::msAlphaDstBlend[AlphaState::DBM_QUANTITY] =
{
	D3DBLEND_ZERO,          // AlphaState::DBM_ZERO
	D3DBLEND_ONE,           // AlphaState::DBM_ONE
	D3DBLEND_SRCCOLOR,      // AlphaState::DBM_SRC_COLOR
	D3DBLEND_INVSRCCOLOR,   // AlphaState::DBM_ONE_MINUS_SRC_COLOR
	D3DBLEND_SRCALPHA,      // AlphaState::DBM_SRC_ALPHA
	D3DBLEND_INVSRCALPHA,   // AlphaState::DBM_ONE_MINUS_SRC_ALPHA
	D3DBLEND_DESTALPHA,     // AlphaState::DBM_DST_ALPHA
	D3DBLEND_INVDESTALPHA,  // AlphaState::DBM_ONE_MINUS_DST_ALPHA
};

//----------------------------------------------------------------------------
void Dx9Renderer::SetAlphaState(AlphaState* pState)
{
	Renderer::SetAlphaState(pState);

	if (pState->BlendEnabled)
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		WIRE_ASSERT(SUCCEEDED(msResult));

		DWORD srcBlend = msAlphaSrcBlend[pState->SrcBlend];
		DWORD dstBlend = msAlphaDstBlend[pState->DstBlend];

		msResult = mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, srcBlend);
		msResult = mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, dstBlend);
		msResult = mpD3DDevice->SetRenderState(D3DRS_BLENDFACTOR, 0);
	}
	else
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
}
