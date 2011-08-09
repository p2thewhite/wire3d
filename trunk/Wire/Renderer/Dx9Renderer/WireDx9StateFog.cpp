// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

const DWORD PdrRendererData::FOG_DENSITY[StateFog::DF_QUANTITY] = 
{
	D3DFOG_LINEAR,  // StateFog::DF_LINEAR
	D3DFOG_EXP,     // StateFog::DF_EXP
	D3DFOG_EXP2,    // StateFog::DF_EXPSQR
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateFog* pState)
{
	mspStates[State::FOG] = pState;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
	if (pState->Enabled)
	{
		hr = rDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&pState->
			Start)));
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&pState->End)));
		WIRE_ASSERT(SUCCEEDED(hr));

		D3DCOLOR fogColor = D3DCOLOR_COLORVALUE(pState->Color.R(),
			pState->Color.G(), pState->Color.B(), 1.0F);
		hr = rDevice->SetRenderState(D3DRS_FOGCOLOR, fogColor);
		WIRE_ASSERT(SUCCEEDED(hr));

		Float density = 1.0F;
		hr = rDevice->SetRenderState(D3DRS_FOGDENSITY,
			*((DWORD*)(&density)));
		WIRE_ASSERT(SUCCEEDED(hr));

//		if (pState->ApplyFunction == StateFog::AF_PER_PIXEL)
// 		{
// 			hr = rDevice->SetRenderState(D3DRS_FOGTABLEMODE,
// 				PdrRendererData::FOG_DENSITY[pState->DensityFunc]);
// 			WIRE_ASSERT(SUCCEEDED(hr));
// 		}
// 		else
// 		{
			hr = rDevice->SetRenderState(D3DRS_FOGVERTEXMODE,
				PdrRendererData::FOG_DENSITY[pState->DensityFunc]);    
			WIRE_ASSERT(SUCCEEDED(hr));
// 		}
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}   
}
