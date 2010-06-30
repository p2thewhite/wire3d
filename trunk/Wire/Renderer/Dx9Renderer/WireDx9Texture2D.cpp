#include "WireDx9Texture2D.h"

#include "WireDx9RendererData.h"
#include "WireRenderer.h"
#include "WireTexture2D.h"
#include <d3dx9tex.h>

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
	UChar* pSrc = pImage->GetData();
	UChar* pDst = 0;
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

	DWORD usage = 0;
	D3DPOOL pool = D3DPOOL_MANAGED;
	const UInt mipmapCount = pImage->GetMipmapCount();

	HRESULT hr;
	hr = D3DXCreateTexture(pRenderer->GetRendererData()->D3DDevice,
		pImage->GetBound(0), pImage->GetBound(1), mipmapCount, usage,
		PdrRendererData::sImage2DFormat[format], pool, &mpTexture);
	WIRE_ASSERT(SUCCEEDED(hr));

	if (pDst)
	{
		IDirect3DSurface9* pD3DSurface = NULL;

		for (UInt i = 0; i < mipmapCount; i++)
		{
			hr = mpTexture->GetSurfaceLevel(i, &pD3DSurface);
			WIRE_ASSERT(SUCCEEDED(hr) && pD3DSurface);

			RECT r;
			UInt pitch	= pImage->GetBound(0, i);
			r.bottom	= pImage->GetBound(1, i);
			r.right		= pitch;
			r.top		= 0;
			r.left		= 0;
			UInt offset = pImage->GetMipmapOffset(i);

			hr = D3DXLoadSurfaceFromMemory(pD3DSurface, NULL, NULL,
				pDst+offset, PdrRendererData::sImage2DFormat[format],
				pitch * bpp, NULL, &r, D3DX_FILTER_NONE, 0xFF000000);
			WIRE_ASSERT(SUCCEEDED(hr));

			pD3DSurface->Release();
		}

		// mark the texture as dirty
		hr = mpTexture->AddDirtyRect(NULL);
		WIRE_ASSERT(SUCCEEDED(hr));
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

	// TODO: introduce ApplyMode
	hr = rDevice->SetTextureStageState(unit, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetTextureStageState(unit, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetTextureStageState(unit, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
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
