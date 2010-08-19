#include "WireGXTexture2D.h"

#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include "WireTexture2D.h"
#include <malloc.h>

using namespace Wire;

UChar PdrRendererData::sImage2DFormat[Image2D::FM_QUANTITY] =
{
	GX_TF_RGBA8,	// Image2D::FM_RGB888
	GX_TF_RGBA8,	// Image2D::FM_RGBA8888
	GX_TF_RGB565,	// Image2D::FM_RGB565
	GX_TF_RGB5A3,	// Image2D::FM_RGBA4444
};

UChar PdrRendererData::sTexMinFilter[Texture2D::FT_QUANTITY] =
{
	GX_NEAR,			// Texture2D::FT_NEAREST
	GX_LINEAR,			// Texture2D::FT_LINEAR
	GX_NEAR_MIP_NEAR,	// Texture2D::FT_NEAREST_NEAREST
	GX_NEAR_MIP_LIN,	// Texture2D::FT_NEAREST_LINEAR
	GX_LIN_MIP_NEAR,	// Texture2D::FT_LINEAR_NEAREST
	GX_LIN_MIP_LIN,		// Texture2D::FT_LINEAR_LINEAR
};

UChar PdrRendererData::sTexWrapMode[Texture2D::WT_QUANTITY] =
{
	GX_CLAMP,	// Texture2D::WT_CLAMP
	GX_REPEAT,	// Texture2D::WT_REPEAT
	GX_MIRROR,	// Texture2D::WT_MIRRORED_REPEAT
};

//----------------------------------------------------------------------------
PdrTexture2D::PdrTexture2D(Renderer* pRenderer, const Texture2D* pTexture)
{
	const Image2D* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	UInt bpp = pImage->GetBytesPerPixel();
	bpp = (bpp == 3) ? 4 : bpp;
	UInt totalMemory = PdrRendererData::GetTotalImageMemory(pImage, bpp);

	mpImage = memalign(32, totalMemory);
	UChar* pDst = static_cast<UChar*>(mpImage);
	WIRE_ASSERT(pDst);
	Image2D::FormatMode format = pImage->GetFormat();

	for (UInt mipLevel = 0; mipLevel < pImage->GetMipmapCount(); mipLevel++)
	{
		UChar* pSrc = pImage->GetMipmap(mipLevel);
		UShort width = pImage->GetBound(0, mipLevel);
		UShort height = pImage->GetBound(1, mipLevel);

		if (pSrc)
		{
			switch (format)
			{
			case Image2D::FM_RGBA8888:
				PdrRendererData::ConvertRGBA8888ToTiles(pSrc, width, height,
					pDst);
				break;

			case Image2D::FM_RGB888:
				PdrRendererData::ConvertRGB888ToTiles(pSrc, width, height,
					pDst);
				break;

			case Image2D::FM_RGB565:
				PdrRendererData::ConvertRGB565ToTiles(pSrc, width, height,
					pDst);
				break;

			case Image2D::FM_RGBA4444:
				PdrRendererData::ConvertRGBA4444ToTiles(pSrc, width, height,
					pDst);
				break;

			default:
				break;
			}
		}

		UInt offset = width * height * bpp;
		pDst += (offset < 32) ? 32 : offset;
	}

	DCStoreRange(mpImage, totalMemory);
	GXInvalidateTexAll();

	UShort width = pImage->GetBound(0);
	UShort height = pImage->GetBound(1);
	UChar usesMipmaps = pImage->HasMipmaps() ? GX_TRUE : GX_FALSE;
	GXInitTexObj(&mTexObj, mpImage, width, height,
		PdrRendererData::sImage2DFormat[format],
		PdrRendererData::sTexWrapMode[pTexture->GetWrapType(0)],
		PdrRendererData::sTexWrapMode[pTexture->GetWrapType(1)],
		usesMipmaps);
}

//----------------------------------------------------------------------------
PdrTexture2D::~PdrTexture2D()
{
	free(mpImage);	// allocated using memalign, not using new
}

//----------------------------------------------------------------------------
void PdrTexture2D::Enable(Renderer* pRenderer, const Texture2D* pTexture,
	UInt unit)
{
	WIRE_ASSERT(unit < pRenderer->GetMaxTextureStages());

	GXSetTexCoordGen(GX_TEXCOORD0 + unit, GX_TG_MTX2x4, GX_TG_TEX0 + unit,
		GX_IDENTITY);
	GXSetNumTexGens(1);

	// TODO:
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0 + unit, GX_TEXMAP0 + unit,
		GX_COLOR0A0);

	GXInitTexObjWrapMode(&mTexObj,
		PdrRendererData::sTexWrapMode[pTexture->GetWrapType(0)],
		PdrRendererData::sTexWrapMode[pTexture->GetWrapType(1)]);

	Float anisotropy = static_cast<UInt>(pTexture->GetAnisotropyValue());
	UChar anisoEnum = GX_ANISO_1;
	UChar doEdgeLod = GX_FALSE;
	if (1.0F < anisotropy && anisotropy <= pRenderer->GetMaxAnisotropy())
	{
		anisoEnum = (anisotropy - 3.0F) > 0.0F ? GX_ANISO_4 : GX_ANISO_2;
		doEdgeLod = GX_TRUE;
	}

	UChar magFilter = pTexture->GetFilterType() == Texture2D::FT_NEAREST ?
		GX_NEAR : GX_LINEAR;
	GXInitTexObjLOD(&mTexObj, PdrRendererData::sTexMinFilter[pTexture->
		GetFilterType()], magFilter, 0, 10.0F, 0, GX_FALSE, doEdgeLod,
		anisoEnum);

	// set texture
	GXLoadTexObj(&mTexObj, GX_TEXMAP0 + unit);
}

//----------------------------------------------------------------------------
void PdrTexture2D::Disable(Renderer* pRenderer, UInt unit)
{
	// TODO
}
