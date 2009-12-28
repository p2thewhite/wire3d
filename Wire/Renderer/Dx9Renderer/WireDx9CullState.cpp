#include "WireDx9Renderer.h"

using namespace Wire;

DWORD Dx9Renderer::msCullType[CullState::CT_QUANTITY] = 
{
	D3DCULL_CCW,  // CullState::FT_CCW (front CCW -> cull backface CCW in DX)
	D3DCULL_CW,   // CullState::FT_CW  (front CW -> cull backface CW in DX)
};

//----------------------------------------------------------------------------
void Dx9Renderer::SetCullState(CullState* pState)
{
	Renderer::SetCullState(pState);

	if (pState->Enabled)
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_CULLMODE,
			msCullType[pState->CullFace]);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
}
