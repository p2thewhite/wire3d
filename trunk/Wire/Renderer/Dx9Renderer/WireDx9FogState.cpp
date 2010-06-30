#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

DWORD PdrRendererData::sFogDensity[FogState::DF_QUANTITY] = 
{
	D3DFOG_LINEAR,  // FogState::DF_LINEAR
	D3DFOG_EXP,     // FogState::DF_EXP
	D3DFOG_EXP2,    // FogState::DF_EXPSQR
};

//----------------------------------------------------------------------------
void Renderer::SetFogState(FogState* pState)
{
	mspStates[GlobalState::FOG] = pState;

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

//		if (pState->ApplyFunction == FogState::AF_PER_PIXEL)
// 		{
// 			hr = rDevice->SetRenderState(D3DRS_FOGTABLEMODE,
// 				PdrRendererData::sFogDensity[pState->DensityFunc]);
// 			WIRE_ASSERT(SUCCEEDED(hr));
// 		}
// 		else
// 		{
			hr = rDevice->SetRenderState(D3DRS_FOGVERTEXMODE,
				PdrRendererData::sFogDensity[pState->DensityFunc]);    
			WIRE_ASSERT(SUCCEEDED(hr));
// 		}
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}   
}
