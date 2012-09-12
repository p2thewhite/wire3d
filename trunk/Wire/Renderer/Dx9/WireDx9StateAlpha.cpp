// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

const DWORD PdrRendererData::StateAlpha::ALPHA_SRC_BLEND[Wire::StateAlpha::
	SBM_QUANTITY] =
{
	D3DBLEND_ZERO,          // StateAlpha::SBM_ZERO
	D3DBLEND_ONE,           // StateAlpha::SBM_ONE
	D3DBLEND_DESTCOLOR,     // StateAlpha::SBM_DST_COLOR
	D3DBLEND_INVDESTCOLOR,  // StateAlpha::SBM_ONE_MINUS_DST_COLOR
	D3DBLEND_SRCALPHA,      // StateAlpha::SBM_SRC_ALPHA
	D3DBLEND_INVSRCALPHA,   // StateAlpha::SBM_ONE_MINUS_SRC_ALPHA
	D3DBLEND_DESTALPHA,     // StateAlpha::SBM_DST_ALPHA
	D3DBLEND_INVDESTALPHA,  // StateAlpha::SBM_ONE_MINUS_DST_ALPHA
};

const DWORD PdrRendererData::StateAlpha::ALPHA_DST_BLEND[Wire::StateAlpha::
	DBM_QUANTITY] =
{
	D3DBLEND_ZERO,          // StateAlpha::DBM_ZERO
	D3DBLEND_ONE,           // StateAlpha::DBM_ONE
	D3DBLEND_SRCCOLOR,      // StateAlpha::DBM_SRC_COLOR
	D3DBLEND_INVSRCCOLOR,   // StateAlpha::DBM_ONE_MINUS_SRC_COLOR
	D3DBLEND_SRCALPHA,      // StateAlpha::DBM_SRC_ALPHA
	D3DBLEND_INVSRCALPHA,   // StateAlpha::DBM_ONE_MINUS_SRC_ALPHA
	D3DBLEND_DESTALPHA,     // StateAlpha::DBM_DST_ALPHA
	D3DBLEND_INVDESTALPHA,  // StateAlpha::DBM_ONE_MINUS_DST_ALPHA
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateAlpha* pState)
{
	WIRE_ASSERT(pState);
	mspStates[State::ALPHA] = pState;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	PdrRendererData::StateAlpha& state = mpData->AlphaState;

	if (!state.IsValid)
	{
		hr = rDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &state.
			ALPHABLENDENABLE);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_SRCBLEND, &state.SRCBLEND);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_DESTBLEND, &state.DESTBLEND);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_BLENDFACTOR, &state.
			ALPHABLENDENABLE);
		WIRE_ASSERT(SUCCEEDED(hr));

		state.IsValid = true;
	}

	if (pState->BlendEnabled)
	{
		if (state.ALPHABLENDENABLE != TRUE)
		{
			hr = rDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.ALPHABLENDENABLE = TRUE;
		}

		DWORD srcBlend = PdrRendererData::StateAlpha::ALPHA_SRC_BLEND[
			pState->SrcBlend];
		if (state.SRCBLEND != srcBlend)
		{
			hr = rDevice->SetRenderState(D3DRS_SRCBLEND, srcBlend);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.SRCBLEND = srcBlend;
		}

		DWORD dstBlend = PdrRendererData::StateAlpha::ALPHA_DST_BLEND[
			pState->DstBlend];
		if (state.DESTBLEND != dstBlend)
		{
			hr = rDevice->SetRenderState(D3DRS_DESTBLEND, dstBlend);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.DESTBLEND = dstBlend;
		}

		if (state.BLENDFACTOR != 0)
		{
			hr = rDevice->SetRenderState(D3DRS_BLENDFACTOR, 0);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.BLENDFACTOR = 0;
		}
	}
	else
	{
		if (state.ALPHABLENDENABLE != FALSE)
		{
			hr = rDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.ALPHABLENDENABLE = FALSE;
		}
	}
}
