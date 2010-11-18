#include "WireRenderer.h"

#include "WireDx9RendererData.h"
#include "WireLight.h"

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

	D3DLIGHT9 d3dLight;
	d3dLight.Ambient.r = pLight->Ambient.R();
	d3dLight.Ambient.g = pLight->Ambient.G();
	d3dLight.Ambient.b = pLight->Ambient.B();
	d3dLight.Ambient.a = 1.0F;

	d3dLight.Diffuse.r = pLight->Color.R();
	d3dLight.Diffuse.g = pLight->Color.G();
	d3dLight.Diffuse.b = pLight->Color.B();
	d3dLight.Diffuse.a = 1.0F;

	d3dLight.Specular.r = 1.0F;
	d3dLight.Specular.g = 1.0F;
	d3dLight.Specular.b = 1.0F;
	d3dLight.Specular.a = 1.0F;

	d3dLight.Attenuation0 = 1.0F;
	d3dLight.Attenuation1 = 0.0F;
	d3dLight.Attenuation2 = 0.0F;

	static Float sqrtMaxReal = MathF::Sqrt(MathF::MAX_REAL);
	d3dLight.Falloff = 1.0F;
	d3dLight.Theta = 0.0F;
	d3dLight.Phi = 0.0F;
	d3dLight.Range = sqrtMaxReal;

	switch (pLight->Type)
	{
	case Light::LT_POINT:
		d3dLight.Type = D3DLIGHT_POINT;
		d3dLight.Position.x = pLight->Position.X();
		d3dLight.Position.y = pLight->Position.Y();
		d3dLight.Position.z = pLight->Position.Z();
		break;

	case Light::LT_DIRECTIONAL:
		d3dLight.Type = D3DLIGHT_DIRECTIONAL;
		WIRE_ASSERT(pLight->Direction.Length() > 0);
		d3dLight.Direction.x = pLight->Direction.X();
		d3dLight.Direction.y = pLight->Direction.Y();
		d3dLight.Direction.z = pLight->Direction.Z();
		break;

	case Light::LT_SPOT:
		d3dLight.Type = D3DLIGHT_SPOT;
		d3dLight.Position.x = pLight->Position.X();
		d3dLight.Position.y = pLight->Position.Y();
		d3dLight.Position.z = pLight->Position.Z();
		d3dLight.Direction.x = pLight->Direction.X();
		d3dLight.Direction.y = pLight->Direction.Y();
		d3dLight.Direction.z = pLight->Direction.Z();
 		d3dLight.Phi = pLight->Angle;
 		d3dLight.Theta = 0.0F;
 		d3dLight.Falloff = pLight->Exponent;
		break;

	default:
		WIRE_ASSERT(false);
	}

	const ColorRGB& rC = pLight->Ambient;
	DWORD ambientColor = D3DCOLOR_COLORVALUE(rC.R(), rC.G(), rC.B(), 1.0F);
	rDevice->SetRenderState(D3DRS_AMBIENT, ambientColor);

	hr = rDevice->SetLight(unit, &d3dLight);
 	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::EnableLighting()
{
	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	hr = rDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	WIRE_ASSERT(SUCCEEDED(hr));
// 	hr = rDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
// 	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::DisableLighting()
{
	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;

	hr = rDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));
// 	hr = rDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
// 	WIRE_ASSERT(SUCCEEDED(hr));
}
