#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetState(MaterialState* pState)
{
	mspStates[GlobalState::MATERIAL] = pState;

	D3DMATERIAL9 material;
	material.Ambient.r = pState->Ambient.R();
	material.Ambient.g = pState->Ambient.G();
	material.Ambient.b = pState->Ambient.B();
	material.Ambient.a = pState->Ambient.A();

	material.Diffuse.r = pState->Ambient.R();
	material.Diffuse.g = pState->Ambient.G();
	material.Diffuse.b = pState->Ambient.B();
	material.Diffuse.a = pState->Ambient.A();

	material.Specular.r = 0.0F;
	material.Specular.g = 0.0F;
	material.Specular.b = 0.0F;
	material.Specular.a = 0.0F;

	material.Emissive.r = 0.0F;
	material.Emissive.g = 0.0F;
	material.Emissive.b = 0.0F;
	material.Emissive.a = 0.0F;

	material.Power = 1.0F;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	hr = rDevice->SetMaterial(&material);
	WIRE_ASSERT(SUCCEEDED(hr));

// These values are set by default. Unless they are changed, we don't need to
// set them here.
//	hr = rDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
//  	WIRE_ASSERT(SUCCEEDED(hr));
// 
// 	hr = rDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
//  	WIRE_ASSERT(SUCCEEDED(hr));
// 
// 	hr = rDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
//  	WIRE_ASSERT(SUCCEEDED(hr));
// 
// 	hr = rDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,
// 		D3DMCS_MATERIAL);
//  	WIRE_ASSERT(SUCCEEDED(hr));
// 
// 	hr = rDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE,
// 		D3DMCS_MATERIAL);
//  	WIRE_ASSERT(SUCCEEDED(hr));
}
