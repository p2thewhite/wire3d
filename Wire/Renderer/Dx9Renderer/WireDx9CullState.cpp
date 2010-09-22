#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

DWORD PdrRendererData::sCullType[CullState::CM_QUANTITY] = 
{
	D3DCULL_NONE,
	D3DCULL_CCW,  // CullState::FT_CCW (front CCW -> cull backface CCW in DX)
	D3DCULL_CW,   // CullState::FT_CW  (front CW -> cull backface CW in DX)
};

//----------------------------------------------------------------------------
void Renderer::SetState(CullState* pState)
{
	mspStates[GlobalState::CULL] = pState;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
	if (pState->Enabled)
	{
		hr = rDevice->SetRenderState(D3DRS_CULLMODE,
			PdrRendererData::sCullType[pState->CullFace]);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}
