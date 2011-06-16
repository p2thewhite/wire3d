// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9Texture2D.h"

#include "WireDx9RendererData.h"
#include "WireImage2D.h"
#include "WireRenderer.h"
#include "WireTexture2D.h"

using namespace Wire;

D3DFORMAT PdrRendererData::sImage2DFormat[Image2D::FM_QUANTITY] =
{
	D3DFMT_A8R8G8B8,	// Image2D::FM_RGB888
	D3DFMT_A8R8G8B8,	// Image2D::FM_RGBA8888
	D3DFMT_R5G6B5,		// Image2D::FM_RGB565
	D3DFMT_A4R4G4B4,	// Image2D::FM_RGBA4444
};

DWORD PdrRendererData::sTexMinFilter[Texture2D::FT_QUANTITY] =
{
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST_NEAREST
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST_LINEAR
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR_LINEAR
};

DWORD PdrRendererData::sTexMipFilter[Texture2D::FT_QUANTITY] =
{
	D3DTEXF_NONE,   // Texture2D::FT_NEAREST
	D3DTEXF_NONE,   // Texture2D::FT_LINEAR
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_NEAREST_LINEAR
	D3DTEXF_POINT,  // Texture2D::FT_LINEAR_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR_LINEAR
};

DWORD PdrRendererData::sTexWrapMode[Texture2D::WT_QUANTITY] =
{
	D3DTADDRESS_CLAMP,      // Texture2D::WT_CLAMP
	D3DTADDRESS_WRAP,       // Texture2D::WT_REPEAT
	D3DTADDRESS_MIRROR,     // Texture2D::WT_MIRRORED_REPEAT
};

//----------------------------------------------------------------------------
PdrTexture2D::PdrTexture2D(Renderer* pRenderer, const Texture2D* pTexture)
{
	// Copy the image data from system memory to video memory.
	const Image2D* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	// Windows stores BGR (lowest byte to highest byte), but Wire stores RGB.
	// The byte ordering must be reversed.
	UChar* const pSrc = pImage->GetData();
	UChar* pDst = NULL;
	UInt quantity = pImage->GetTotalQuantity();
	UInt bpp = pImage->GetBytesPerPixel();
	bpp = (bpp == 3) ? 4 : bpp;
	UInt byteQuantity = quantity * bpp;
	Image2D::FormatMode format = pImage->GetFormat();

	if (pSrc)
	{
		UInt srcOffset = 0;
		UInt dstOffset = 0;

		switch (format)
		{
		case Image2D::FM_RGB888:
			// Swap R and B and pad to an RGBA image.
			pDst = WIRE_NEW UChar[byteQuantity];
			for (UInt i = 0; i < quantity; i++)
			{
				pDst[dstOffset    ] = pSrc[srcOffset + 2];
				pDst[dstOffset + 1] = pSrc[srcOffset + 1];
				pDst[dstOffset + 2] = pSrc[srcOffset    ];
				pDst[dstOffset + 3] = 0xFF;
				srcOffset += 3;
				dstOffset += 4;
			}

			break;

		case Image2D::FM_RGBA8888:
			pDst = WIRE_NEW UChar[byteQuantity];
			for (UInt i = 0; i < quantity; i++)
			{
				pDst[srcOffset    ] = pSrc[srcOffset + 2];
				pDst[srcOffset + 1] = pSrc[srcOffset + 1];
				pDst[srcOffset + 2] = pSrc[srcOffset    ];
				pDst[srcOffset + 3] = pSrc[srcOffset + 3];
				srcOffset += 4;
			}

			break;

		case Image2D::FM_RGB565:
			pDst = WIRE_NEW UChar[byteQuantity];
			for (UInt i = 0; i < quantity; i++)
			{
				pDst[srcOffset] = pSrc[srcOffset+1];
				pDst[srcOffset+1] = pSrc[srcOffset];
				srcOffset += 2;
			}

			break;

		case Image2D::FM_RGBA4444:
			pDst = WIRE_NEW UChar[byteQuantity];
			for (UInt i = 0; i < quantity; i++)
			{
				UShort texel = static_cast<UShort>(pSrc[srcOffset]);
				texel = texel << 8;
				texel |= static_cast<UShort>(pSrc[srcOffset+1]);
				UShort texelRGB = (texel & 0xFFF0) >> 4;
				UShort texelA = (texel & 0x0F) << 12;
				texel = texelRGB | texelA;
				pDst[srcOffset] = static_cast<UChar>(texel);
				pDst[srcOffset+1] = static_cast<UChar>(texel >> 8);
				srcOffset += 2;
			}

			break;

		default:
			// There is no need to preprocess depth or intensity images. The
			// floating-point formats and the 16/32-bit integer formats are
			// already RGB/RGBA.
			pDst = pSrc;
			break;
		}
	}

	const D3DPOOL pool = (pTexture->GetUsage() == Buffer::UT_STATIC) ?
		D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	const UInt mipmapCount = pImage->GetMipmapCount();
	const DWORD usage = 0;

	HRESULT hr;
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	hr = rDevice->CreateTexture(pImage->GetBound(0), pImage->GetBound(1),
		mipmapCount, usage, PdrRendererData::sImage2DFormat[format], pool,
		&mpTexture, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));

	if (pDst)
	{
		Buffer::LockingMode lockingMode = pTexture->GetUsage() ==
			Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY;

		for (UInt level = 0; level < mipmapCount; ++level)
		{
			void* pData = Lock(lockingMode, level);
			WIRE_ASSERT(pData);
			UInt size = pImage->GetQuantity(level) * bpp;
			UInt offset = pImage->GetMipmapOffset(level) / pImage->
				GetBytesPerPixel();
			System::Memcpy(pData, size, pDst + offset * bpp, size);
			Unlock(level);
		}
	}

	if (pSrc != pDst)
	{
		WIRE_DELETE[] pDst;
	}
}

//----------------------------------------------------------------------------
PdrTexture2D::~PdrTexture2D()
{
	HRESULT hr;
	hr = mpTexture->Release();
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrTexture2D::Enable(Renderer* pRenderer, const Texture2D* pTexture,
	UInt unit)
{
	WIRE_ASSERT(unit < pRenderer->GetMaxTextureStages());

	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;

	// Anisotropic filtering value.
	Float anisotropy = pTexture->GetAnisotropyValue();
	if (1.0F < anisotropy && anisotropy <= pRenderer->GetMaxAnisotropy())
	{
		hr = rDevice->SetSamplerState(unit, D3DSAMP_MAXANISOTROPY,
			static_cast<DWORD>(anisotropy));
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else if (1.0F < anisotropy)
	{
		hr = rDevice->SetSamplerState(unit, D3DSAMP_MAXANISOTROPY,
			static_cast<DWORD>(pRenderer->GetMaxAnisotropy()));
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetSamplerState(unit, D3DSAMP_MAXANISOTROPY,
			static_cast<DWORD>(1.0F));
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	// Set the filter mode.
	Texture2D::FilterType filterType = pTexture->GetFilterType();
	if (filterType == Texture2D::FT_NEAREST)
	{
		hr = rDevice->SetSamplerState(unit, D3DSAMP_MAGFILTER,
			D3DTEXF_POINT);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		if (1.0F < anisotropy && anisotropy <= pRenderer->GetMaxAnisotropy())
		{
			hr = rDevice->SetSamplerState(unit, D3DSAMP_MAGFILTER,
				D3DTEXF_ANISOTROPIC);
			WIRE_ASSERT(SUCCEEDED(hr));
		}
		else
		{
			hr = rDevice->SetSamplerState(unit, D3DSAMP_MAGFILTER,
				D3DTEXF_LINEAR);
			WIRE_ASSERT(SUCCEEDED(hr));
		}
	}

	// Set the mipmap mode.
	if (1.0F < anisotropy && anisotropy <= pRenderer->GetMaxAnisotropy())
	{
		hr = rDevice->SetSamplerState(unit, D3DSAMP_MINFILTER,
			D3DTEXF_ANISOTROPIC);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->SetSamplerState(unit, D3DSAMP_MIPFILTER,
			D3DTEXF_ANISOTROPIC);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetSamplerState(unit, D3DSAMP_MINFILTER,
			PdrRendererData::sTexMinFilter[filterType]);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->SetSamplerState(unit, D3DSAMP_MIPFILTER,
			PdrRendererData::sTexMipFilter[filterType]);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	// Set the border color (for clamp to border).
	const ColorRGBA borderColor(ColorRGBA::BLACK); //pTexture->GetBorderColor();
	hr = rDevice->SetSamplerState(unit, D3DSAMP_BORDERCOLOR,
		D3DCOLOR_COLORVALUE(borderColor.R(), borderColor.G(), borderColor.B(),
		borderColor.A()));
	WIRE_ASSERT(SUCCEEDED(hr));

	hr = rDevice->SetSamplerState(unit, D3DSAMP_ADDRESSU,
		PdrRendererData::sTexWrapMode[pTexture->GetWrapType(0)]);
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetSamplerState(unit, D3DSAMP_ADDRESSV,
		PdrRendererData::sTexWrapMode[pTexture->GetWrapType(1)]);
	WIRE_ASSERT(SUCCEEDED(hr));

	hr = rDevice->SetTexture(unit, mpTexture);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrTexture2D::Disable(Renderer* pRenderer, UInt unit)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->SetTextureStageState(unit, D3DTSS_COLOROP, D3DTOP_DISABLE);
	WIRE_ASSERT(SUCCEEDED(hr));

	hr = rDevice->SetTexture(unit, 0);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void* PdrTexture2D::Lock(Buffer::LockingMode mode, UInt level)
{
	D3DLOCKED_RECT rect;
	HRESULT hr;
	hr = mpTexture->LockRect(level, &rect, NULL,
		PdrRendererData::sBufferLocking[mode]);
	WIRE_ASSERT(SUCCEEDED(hr));
	return rect.pBits;
}

//----------------------------------------------------------------------------
void PdrTexture2D::Unlock(UInt level)
{
	HRESULT hr;
	hr = mpTexture->UnlockRect(level);
	WIRE_ASSERT(SUCCEEDED(hr));
}
