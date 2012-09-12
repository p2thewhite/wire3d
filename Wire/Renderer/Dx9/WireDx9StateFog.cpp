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

const DWORD PdrRendererData::StateFog::FOG_DENSITY[Wire::StateFog::
	DF_QUANTITY] = 
{
	D3DFOG_LINEAR,  // StateFog::DF_LINEAR
	D3DFOG_EXP,     // StateFog::DF_EXP
	D3DFOG_EXP2,    // StateFog::DF_EXPSQR
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateFog* pState)
{
	WIRE_ASSERT(pState);
	mspStates[State::FOG] = pState;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	PdrRendererData::StateFog& state = mpData->FogState;
	if (!state.IsValid)
	{
		hr = rDevice->GetRenderState(D3DRS_FOGENABLE, &state.FOGENABLE);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_FOGSTART, &state.FOGSTART);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_FOGEND, &state.FOGEND);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_FOGCOLOR, &state.FOGCOLOR);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_FOGDENSITY, &state.FOGDENSITY);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->GetRenderState(D3DRS_FOGVERTEXMODE, &state.
			FOGVERTEXMODE);
		WIRE_ASSERT(SUCCEEDED(hr));

		state.IsValid = true;
	}

	if (pState->Enabled)
	{
		if (state.FOGENABLE != TRUE)
		{
			hr = rDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.FOGENABLE = TRUE;
		}

		DWORD fogStart = *((DWORD*)(&pState->Start));
		if (state.FOGSTART != fogStart)
		{
			hr = rDevice->SetRenderState(D3DRS_FOGSTART, fogStart);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.FOGSTART = fogStart;
		}

		DWORD fogEnd = *((DWORD*)(&pState->End));
		if (state.FOGEND != fogEnd)
		{
			hr = rDevice->SetRenderState(D3DRS_FOGEND, fogEnd);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.FOGEND = fogEnd;
		}

		D3DCOLOR fogColor = D3DCOLOR_COLORVALUE(pState->Color.R(),
			pState->Color.G(), pState->Color.B(), 1.0F);
		if (state.FOGCOLOR != fogColor)
		{
			hr = rDevice->SetRenderState(D3DRS_FOGCOLOR, fogColor);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.FOGCOLOR = fogColor;
		}

		Float density = 1.0F;
		DWORD dDensity = *((DWORD*)(&density));
		if (state.FOGDENSITY != dDensity)
		{
			hr = rDevice->SetRenderState(D3DRS_FOGDENSITY, dDensity);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.FOGDENSITY = dDensity;
		}

//		if (pState->ApplyFunction == StateFog::AF_PER_PIXEL)
// 		{
// 			hr = rDevice->SetRenderState(D3DRS_FOGTABLEMODE,
// 				PdrRendererData::FOG_DENSITY[pState->DensityFunc]);
// 			WIRE_ASSERT(SUCCEEDED(hr));
// 		}
// 		else
// 		{
			DWORD densityFunc = PdrRendererData::StateFog::FOG_DENSITY[
				pState->DensityFunc];
			if (state.FOGVERTEXMODE != densityFunc)
			{
				hr = rDevice->SetRenderState(D3DRS_FOGVERTEXMODE,
					densityFunc);    
				WIRE_ASSERT(SUCCEEDED(hr));
				state.FOGVERTEXMODE = densityFunc;
			}
// 		}
	}
	else
	{
		if (state.FOGENABLE != FALSE)
		{
			hr = rDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
			WIRE_ASSERT(SUCCEEDED(hr));
			state.FOGENABLE = FALSE;
		}
	}   
}
