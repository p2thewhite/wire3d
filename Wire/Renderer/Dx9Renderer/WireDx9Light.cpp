#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetLight(const Light* pLight, UInt unit)
{
	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	if (!pLight)
	{
		hr = rDevice->LightEnable(unit, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
		return;
	}

	hr = rDevice->LightEnable(unit, TRUE);
	WIRE_ASSERT(SUCCEEDED(hr));

	static Float sqrtMaxReal = MathF::Sqrt(MathF::MAX_REAL);

	D3DLIGHT9 d3dLight;
	d3dLight.Falloff = 1.0f;
	d3dLight.Theta = 0.0f;
	d3dLight.Phi = 0.0f;
	d3dLight.Range = sqrtMaxReal;


// 	hr = rDevice->SetLight(unit, &d3dLight);
// 	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::EnableLighting()
{
	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	hr = rDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::DisableLighting()
{
	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	hr = rDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));
}
