#include "WireGXTexture2D.h"

#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include "WireTexture2D.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrTexture2D::PdrTexture2D(Renderer* pRenderer, const Texture2D* pTexture)
{
	const Image2D* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	PdrRendererData& rPdrData = *(pRenderer->GetRendererData());

	UInt bpp = pImage->GetBytesPerPixel();
	bpp = (bpp == 3) ? 4 : bpp;
	UInt totalMemory = rPdrData.GetTotalImageMemory(pImage, bpp);

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
				rPdrData.ConvertRGBA8888ToTiles(pSrc, width, height, pDst);
				break;

			case Image2D::FM_RGB888:
				rPdrData.ConvertRGB888ToTiles(pSrc, width, height, pDst);
				break;

			case Image2D::FM_RGB565:
				rPdrData.ConvertRGB565ToTiles(pSrc, width, height, pDst);
				break;

			case Image2D::FM_RGBA4444:
				rPdrData.ConvertRGBA4444ToTiles(pSrc, width, height, pDst);
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
		PdrRendererData::msImage2DFormat[format],
		PdrRendererData::msTexWrapMode[pTexture->GetWrapType(0)],
		PdrRendererData::msTexWrapMode[pTexture->GetWrapType(1)],
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
	GXSetTexCoordGen(GX_TEXCOORD0 + unit, GX_TG_MTX2x4, GX_TG_TEX0 + unit,
		GX_IDENTITY);
	GXSetNumTexGens(1);

	// TODO:
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0 + unit, GX_TEXMAP0 + unit,
		GX_COLOR0A0);

	GXInitTexObjWrapMode(&mTexObj,
		PdrRendererData::msTexWrapMode[pTexture->GetWrapType(0)],
		PdrRendererData::msTexWrapMode[pTexture->GetWrapType(1)]);

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
	GXInitTexObjLOD(&mTexObj, PdrRendererData::msTexMinFilter[pTexture->
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
