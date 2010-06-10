#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

DWORD PdrRendererData::msAlphaSrcBlend[AlphaState::SBM_QUANTITY] =
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

DWORD PdrRendererData::msAlphaDstBlend[AlphaState::DBM_QUANTITY] =
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
void Renderer::SetAlphaState(AlphaState* pState)
{
	mspStates[GlobalState::ALPHA] = pState;

	IDirect3DDevice9*& rDevice = mpData->mpD3DDevice;
	HRESULT hr;
	if (pState->BlendEnabled)
	{
		hr = rDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		WIRE_ASSERT(SUCCEEDED(hr));

		DWORD srcBlend = PdrRendererData::msAlphaSrcBlend[pState->SrcBlend];
		DWORD dstBlend = PdrRendererData::msAlphaDstBlend[pState->DstBlend];

		hr = rDevice->SetRenderState(D3DRS_SRCBLEND, srcBlend);
		WIRE_ASSERT(SUCCEEDED(hr));
		hr = rDevice->SetRenderState(D3DRS_DESTBLEND, dstBlend);
		WIRE_ASSERT(SUCCEEDED(hr));
		hr = rDevice->SetRenderState(D3DRS_BLENDFACTOR, 0);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}
