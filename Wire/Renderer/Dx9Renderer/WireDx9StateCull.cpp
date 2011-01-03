#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

DWORD PdrRendererData::sCullType[StateCull::CM_QUANTITY] = 
{
	D3DCULL_NONE,
	D3DCULL_CCW,  // StateCull::CM_FRONT(front CCW -> cull backface CCW in DX)
	D3DCULL_CW,   // StateCull::CM_BACK (front CW -> cull backface CW in DX)
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateCull* pState)
{
	mspStates[State::CULL] = pState;

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
