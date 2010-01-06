#include "WireDx9Renderer.h"

using namespace Wire;

DWORD Dx9Renderer::msFogDensity[FogState::DF_QUANTITY] = 
{
	D3DFOG_LINEAR,  // FogState::DF_LINEAR
	D3DFOG_EXP,     // FogState::DF_EXP
	D3DFOG_EXP2,    // FogState::DF_EXPSQR
};

//----------------------------------------------------------------------------
void Dx9Renderer::SetFogState(FogState* pState)
{
	Renderer::SetFogState(pState);

	if (pState->Enabled)
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		WIRE_ASSERT(SUCCEEDED(msResult));

		msResult = mpD3DDevice->SetRenderState(D3DRS_FOGSTART,
			*((DWORD*)(&pState->Start)));
		WIRE_ASSERT(SUCCEEDED(msResult));

		msResult = mpD3DDevice->SetRenderState(D3DRS_FOGEND,
			*((DWORD*)(&pState->End)));
		WIRE_ASSERT(SUCCEEDED(msResult));

		D3DCOLOR fogColor = D3DCOLOR_COLORVALUE(pState->Color.R(),
			pState->Color.G(), pState->Color.B(), 1.0F);
		msResult = mpD3DDevice->SetRenderState(D3DRS_FOGCOLOR, fogColor);
		WIRE_ASSERT(SUCCEEDED(msResult));

		Float density = 1.0F;
		msResult = mpD3DDevice->SetRenderState(D3DRS_FOGDENSITY,
			*((DWORD*)(&density)));
		WIRE_ASSERT(SUCCEEDED(msResult));

//		if (pState->ApplyFunction == FogState::AF_PER_PIXEL)
// 		{
// 			msResult = mpD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE,
// 				msFogDensity[pState->DensityFunc]);
// 			WIRE_ASSERT(SUCCEEDED(msResult));
// 		}
// 		else
// 		{
			msResult = mpD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE,
				msFogDensity[pState->DensityFunc]);    
			WIRE_ASSERT(SUCCEEDED(msResult));
// 		}
	}
	else
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}   
}
