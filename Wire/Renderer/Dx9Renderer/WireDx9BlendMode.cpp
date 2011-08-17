// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireDx9RendererData.h"
#include "WireImage2D.h"

using namespace Wire;

const DWORD PdrRendererData::TEX_BLEND[Material::BM_QUANTITY * 8] =
{
	// Material::BM_REPLACE
	1, D3DTOP_SELECTARG1, D3DTA_TEXTURE, 0,
	1, D3DTOP_SELECTARG1, D3DTA_TEXTURE, 0,

	// Material::BM_MODULATE
	2, D3DTOP_MODULATE, D3DTA_TEXTURE, 0,
	2, D3DTOP_MODULATE, D3DTA_TEXTURE, 0,

	// Material::BM_PASS
	1, D3DTOP_SELECTARG1, D3DTA_CURRENT, 0,
	1, D3DTOP_SELECTARG1, D3DTA_CURRENT, 0,

	// Material::BM_BLEND
	3, D3DTOP_LERP, D3DTA_TFACTOR, D3DTA_TEXTURE,
	2, D3DTOP_MODULATE, D3DTA_TEXTURE, 0,

	// Material::BM_DECAL
	2, D3DTOP_BLENDTEXTUREALPHA, D3DTA_TEXTURE, 0,
	1, D3DTOP_SELECTARG1, D3DTA_CURRENT, 0

	// Material::BM_ADD
	// 	2, D3DTOP_ADD, D3DTA_TEXTURE, 0,
	// 	2, D3DTOP_ADD, D3DTA_TEXTURE, 0
};

//----------------------------------------------------------------------------
void Renderer::SetBlendMode(Material::BlendMode blendMode, UInt unit,
	Bool hasAlpha)
{
	IDirect3DDevice9*& rDevice = GetRendererData()->D3DDevice;
	HRESULT hr;

	UInt idx = blendMode * 8;
	const UInt argColorCount = PdrRendererData::TEX_BLEND[idx];

	DWORD colorOp = PdrRendererData::TEX_BLEND[idx+1];
	hr = rDevice->SetTextureStageState(unit, D3DTSS_COLOROP, colorOp);
	WIRE_ASSERT(SUCCEEDED(hr));

	DWORD colorA1 = PdrRendererData::TEX_BLEND[idx+2];
	hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG1, colorA1);
	WIRE_ASSERT(SUCCEEDED(hr));

	if (argColorCount > 1)
	{
		DWORD op = (unit > 0) ? D3DTA_CURRENT : D3DTA_DIFFUSE;
		hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG2, op);
		WIRE_ASSERT(SUCCEEDED(hr));

		if (argColorCount > 2)
		{
			DWORD colorA0 = PdrRendererData::TEX_BLEND[idx+3];
			hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG0,
				colorA0);
			WIRE_ASSERT(SUCCEEDED(hr));
		}
	}

	if (!hasAlpha)
	{
		idx = Material::BM_PASS * 8;
	}

	const UInt argAlphaCount = PdrRendererData::TEX_BLEND[idx+4];
	DWORD alphaOp = PdrRendererData::TEX_BLEND[idx+5];
	hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAOP, alphaOp);
	WIRE_ASSERT(SUCCEEDED(hr));

	DWORD alphaA0 = PdrRendererData::TEX_BLEND[idx+6];
	hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAARG1, alphaA0);
	WIRE_ASSERT(SUCCEEDED(hr));

	if (argAlphaCount > 1)
	{
		DWORD op = (unit > 0) ? D3DTA_CURRENT : D3DTA_DIFFUSE;
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
