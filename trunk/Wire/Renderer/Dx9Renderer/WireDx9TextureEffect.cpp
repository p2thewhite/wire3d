#include "WireRenderer.h"

#include "WireDx9RendererData.h"
#include "WireTextureEffect.h"

using namespace Wire;

DWORD PdrRendererData::sTexBlend[TextureEffect::BM_QUANTITY] =
{
	D3DTOP_SELECTARG1,		// TextureEffect::BM_REPLACE
	D3DTOP_MODULATE,		// TextureEffect::BM_MODULATE
	D3DTOP_SELECTARG2,		// TextureEffect::BM_PASS
};

//----------------------------------------------------------------------------
void Renderer::ApplyEffect(TextureEffect* pEffect)
{
	WIRE_ASSERT(pEffect->Textures.GetQuantity() ==
		pEffect->BlendOps.GetQuantity());

	IDirect3DDevice9*& rDevice = GetRendererData()->D3DDevice;
	HRESULT hr;

	for (UInt unit = 0; unit < pEffect->BlendOps.GetQuantity(); unit++)
	{
		DWORD colorOp = PdrRendererData::sTexBlend[pEffect->BlendOps[unit]];
		hr = rDevice->SetTextureStageState(unit, D3DTSS_COLOROP, colorOp);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG1,
			D3DTA_TEXTURE);
		WIRE_ASSERT(SUCCEEDED(hr));

		DWORD blendOp = (unit > 0) ? D3DTA_CURRENT : D3DTA_DIFFUSE;
		hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG2, blendOp);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAOP,
			D3DTOP_DISABLE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}
