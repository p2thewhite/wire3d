#include "WireRenderer.h"

#include "WireDx9RendererData.h"
#include "WireImage2D.h"
#include "WireTextureEffect.h"

using namespace Wire;

DWORD PdrRendererData::sTexBlend[TextureEffect::BM_QUANTITY * 8] =
{
	// TextureEffect::BM_REPLACE
	1, D3DTOP_SELECTARG1, D3DTA_TEXTURE, 0,
	1, D3DTOP_SELECTARG1, D3DTA_TEXTURE, 0,

	// TextureEffect::BM_MODULATE
	2, D3DTOP_MODULATE, D3DTA_TEXTURE, 0,
	2, D3DTOP_MODULATE, D3DTA_TEXTURE, 0,

	// TextureEffect::BM_PASS
	1, D3DTOP_SELECTARG1, D3DTA_CURRENT, 0,
	1, D3DTOP_SELECTARG1, D3DTA_CURRENT, 0,

	// TextureEffect::BM_BLEND
	3, D3DTOP_LERP, D3DTA_TFACTOR, D3DTA_TEXTURE,
	2, D3DTOP_MODULATE, D3DTA_TEXTURE, 0,

	// TextureEffect::BM_DECAL
	2, D3DTOP_BLENDTEXTUREALPHA, D3DTA_TEXTURE, 0,
	1, D3DTOP_SELECTARG1, D3DTA_CURRENT, 0

	// TextureEffect::BM_ADD
// 	2, D3DTOP_ADD, D3DTA_TEXTURE, 0,
// 	2, D3DTOP_ADD, D3DTA_TEXTURE, 0
};

//----------------------------------------------------------------------------
void Renderer::ApplyEffect(const TextureEffect* pEffect)
{
	WIRE_ASSERT(pEffect->Textures.GetQuantity() ==
		pEffect->BlendOps.GetQuantity());

	IDirect3DDevice9*& rDevice = GetRendererData()->D3DDevice;
	HRESULT hr;

	for (UInt unit = 0; unit < pEffect->BlendOps.GetQuantity(); unit++)
	{
		UInt idx = pEffect->BlendOps[unit] * 8;
		const UInt argColorCount = PdrRendererData::sTexBlend[idx];

		DWORD colorOp = PdrRendererData::sTexBlend[idx+1];
		hr = rDevice->SetTextureStageState(unit, D3DTSS_COLOROP, colorOp);
		WIRE_ASSERT(SUCCEEDED(hr));

		DWORD colorA1 = PdrRendererData::sTexBlend[idx+2];
		hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG1, colorA1);
		WIRE_ASSERT(SUCCEEDED(hr));

		if (argColorCount > 1)
		{
			DWORD op = (unit > 0) ? D3DTA_CURRENT : D3DTA_DIFFUSE;
			hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG2, op);
			WIRE_ASSERT(SUCCEEDED(hr));

			if (argColorCount > 2)
			{
				DWORD colorA0 = PdrRendererData::sTexBlend[idx+3];
				hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG0,
					colorA0);
				WIRE_ASSERT(SUCCEEDED(hr));
			}
		}

		WIRE_ASSERT(pEffect->Textures[unit]);
		WIRE_ASSERT(pEffect->Textures[unit]->GetImage());
		if (!pEffect->Textures[unit]->GetImage()->HasAlpha())
		{
			idx = TextureEffect::BM_PASS * 8;
		}

		const UInt argAlphaCount = PdrRendererData::sTexBlend[idx+4];
		DWORD alphaOp = PdrRendererData::sTexBlend[idx+5];
		hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAOP, alphaOp);
		WIRE_ASSERT(SUCCEEDED(hr));

		DWORD alphaA0 = PdrRendererData::sTexBlend[idx+6];
		hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAARG1, alphaA0);
		WIRE_ASSERT(SUCCEEDED(hr));

		if (argAlphaCount > 1)
		{
			DWORD op = PdrRendererData::sTexBlend[idx+5];
			hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAARG2, op);
			WIRE_ASSERT(SUCCEEDED(hr));

			if (argAlphaCount > 2)
			{
				DWORD alphaA0 = (unit > 0) ? D3DTA_CURRENT : D3DTA_DIFFUSE;
				hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAARG0,
					alphaA0);
				WIRE_ASSERT(SUCCEEDED(hr));
			}
		}
	}
}
