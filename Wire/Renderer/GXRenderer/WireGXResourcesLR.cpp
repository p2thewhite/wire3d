#include "WireGXRenderer.h"

#include "WireGeometry.h"
#include "WireGXResources.h"
#include "WireTexture.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

UChar GXRenderer::msImageFormat[Image::FM_QUANTITY] =
{
	GX_TF_RGBA8,	// Image::FM_RGB888
	GX_TF_RGBA8,	// Image::FM_RGBA8888
	GX_TF_RGB565,	// Image::FM_RGB565
	GX_TF_RGB5A3,	// Image::FM_RGBA4444
};

//----------------------------------------------------------------------------
void GXRenderer::OnLoadVBuffer(ResourceIdentifier*& rID,
	VertexBuffer* pVBuffer)
{
	VBufferID* pResource = WIRE_NEW VBufferID;
	rID = pResource;

	TArray<VBufferID::VertexElement>& rElements = pResource->Elements;
	VBufferID::VertexElement element;
	UInt vertexCount = pVBuffer->GetVertexQuantity();

	const VertexAttributes& rIAttr = pVBuffer->GetAttributes();
	UInt channels = rIAttr.GetPositionChannels();
	if (channels > 0)
	{
		element.Stride = channels * sizeof(Float);
		element.Size = element.Stride * vertexCount;
 		element.Data = memalign(32, element.Size);
 		element.Attr = GX_VA_POS;
 		element.CompCnt = GX_POS_XYZ;
 		element.CompType = GX_F32;
 		rElements.Append(element);
	}

	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
		{
			element.Stride = sizeof(UInt);
			element.Size = element.Stride * vertexCount;
			element.Data = memalign(32, element.Size);
			element.Attr = GX_VA_CLR0 + unit;
			element.CompCnt = GX_CLR_RGBA;
			element.CompType = GX_RGBA8;
			rElements.Append(element);
		}
	}

	for (UInt unit = 0; unit < rIAttr.GetTCoordChannelQuantity(); unit++)
	{
		channels = rIAttr.GetTCoordChannels(unit);
		if (channels > 0)
		{
			element.Stride = channels * sizeof(Float);
			element.Size = element.Stride * vertexCount;
			element.Data = memalign(32, element.Size);
			element.Attr = GX_VA_TEX0 + unit;
			element.CompCnt = GX_TEX_ST;
			element.CompType = GX_F32;
			rElements.Append(element);
		}
	}

	WIRE_ASSERT(pResource->Elements.GetQuantity() > 0);
	Convert(pVBuffer, pResource);

 	for (UInt i = 0; i < rElements.GetQuantity(); i++)
 	{
 		// TODO: use DCStoreRangeNoSync to accumulate stores, then PPCSync()
 		DCStoreRange(rElements[i].Data, rElements[i].Size);
 	}

	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();
}

//----------------------------------------------------------------------------
void GXRenderer::Convert(const VertexBuffer* pSrc, VBufferID* pResource)
{
	const VertexAttributes& rIAttr = pSrc->GetAttributes();
	TArray<VBufferID::VertexElement>& rElements = pResource->Elements;

	for (UInt i = 0; i < pSrc->GetVertexQuantity(); i++)
	{
		UInt index = 0;

		if (rIAttr.GetPositionChannels() > 0)
		{
			const Float* pPosition = pSrc->GetPosition(i);
			Float* pDst = static_cast<Float*>(rElements[index++].Data);
			UInt channelCount = rIAttr.GetPositionChannels();

			for (UInt k = 0; k < channelCount; k++)
			{
				pDst[i*channelCount+k] = pPosition[k];
			}
		}

		UInt colorChannelQuantity = rIAttr.GetColorChannelQuantity();
		for (UInt unit = 0; unit < colorChannelQuantity; unit++)
		{
			if (rIAttr.GetColorChannels(unit) > 0)
			{
				const Float* pColor = pSrc->GetColor(i, unit);
				UInt color = 0xFFFFFFFF;
				for (UInt k = 0; k < rIAttr.GetColorChannels(unit); k++)
				{
					color = color << 8;
					color |= static_cast<UChar>(pColor[k] * 255.0F);
				}

				if (rIAttr.GetColorChannels(unit) == 3)
				{
					color = color << 8;
					color |= 0xFF;
				}

				UInt* pDst = static_cast<UInt*>(rElements[index++].Data);
 				pDst[i] = color;
			}
		}

		UInt tChannelQuantity = rIAttr.GetTCoordChannelQuantity();
		for (UInt unit = 0; unit < tChannelQuantity; unit++)
		{
			if (rIAttr.GetTCoordChannels(unit) > 0)
			{
				const Float* pUv = pSrc->GetTCoord(i, unit);
				Float* pDst = static_cast<Float*>(rElements[index++].Data);
				UInt channelCount = rIAttr.GetTCoordChannels(unit);

				for (UInt k = 0; k < channelCount; k++)
				{
					pDst[i*channelCount+k] = pUv[k];
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void GXRenderer::OnReleaseVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);
	TArray<VBufferID::VertexElement>& rElements = pResource->Elements;
	for (UInt i = 0; i < rElements.GetQuantity(); i++)
	{
		free(rElements[i].Data);	// allocated using memalign, not using new
	}

	TArray<VBufferID::DisplayList>& rDisplayLists = pResource->
		DisplayLists;
	for (UInt i = 0; i < rDisplayLists.GetQuantity(); i++)
	{
		free(rDisplayLists[i].DL);	// allocated using memalign, not using new
	
		TArray<VBufferID*>& rVBufferIDs = rDisplayLists[i].RegisteredIBuffer->
			RegisteredVBuffers;
		for (UInt j = 0; j < rVBufferIDs.GetQuantity(); j++)
		{
			if (rVBufferIDs[j] == pResource)
			{
				rVBufferIDs.Remove(j);
				break;
			}
		}
	}
	
	WIRE_DELETE pResource;
}

//----------------------------------------------------------------------------
void GXRenderer::OnLoadIBuffer(ResourceIdentifier*& rID, IndexBuffer* pBuffer)
{
	// The texture is encountered the first time. Set up the texture unit
	// in hardware that will manage this texture.
	IBufferID* pResource = WIRE_NEW IBufferID;
	rID = pResource;
}

//----------------------------------------------------------------------------
void GXRenderer::OnReleaseIBuffer(ResourceIdentifier* pID)
{
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	TArray<VBufferID*>& rVBufferIDs = pResource->RegisteredVBuffers; 
	for (UInt i = 0; i < rVBufferIDs.GetQuantity(); i++)
	{
		TArray<VBufferID::DisplayList>& rDisplayLists = rVBufferIDs[i]->
			DisplayLists;

		for (UInt j = 0; j < rDisplayLists.GetQuantity(); j++)
		{
			if (rDisplayLists[j].RegisteredIBuffer == pResource)
			{
				free(rDisplayLists[i].DL);	// allocated using memalign
				rDisplayLists.Remove(j);
				break;
			}
		}
	}

	WIRE_DELETE pResource;
}

//----------------------------------------------------------------------------
void GXRenderer::OnLoadTexture(ResourceIdentifier*& rID, Texture* pTexture)
{
	// The texture is encountered the first time. Set up the texture unit
	// in hardware that will manage this texture.
	TextureID* pResource = WIRE_NEW TextureID;
	pResource->TextureObject = pTexture;
	rID = pResource;

	const Image* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	UInt bpp = pImage->GetBytesPerPixel();
	bpp = (bpp == 3) ? 4 : bpp;
	UInt totalMemory = GetTotalImageMemory(pImage, bpp);

	pResource->Image = memalign(32, totalMemory);
	UChar* pDst = static_cast<UChar*>(pResource->Image);
	WIRE_ASSERT(pDst);
	Image::FormatMode format = pImage->GetFormat();

	for (UInt mipLevel = 0; mipLevel < pImage->GetMipmapCount(); mipLevel++)
	{
		UChar* pSrc = pImage->GetMipmap(mipLevel);
		UShort width = pImage->GetBound(0, mipLevel);
		UShort height = pImage->GetBound(1, mipLevel);

		if (pSrc)
		{
			switch (format)
			{
			case Image::FM_RGBA8888:
				ConvertRGBA8888ToTiles(pSrc, width, height, pDst);
				break;

			case Image::FM_RGB888:
				ConvertRGB888ToTiles(pSrc, width, height, pDst);
				break;

			case Image::FM_RGB565:
				ConvertRGB565ToTiles(pSrc, width, height, pDst);
				break;

			case Image::FM_RGBA4444:
				ConvertRGBA4444ToTiles(pSrc, width, height, pDst);
				break;

			default:
				break;
			}
		}

		UInt offset = width * height * bpp;
		pDst += (offset < 32) ? 32 : offset;
	}

	DCStoreRange(pResource->Image, totalMemory);
	GXInvalidateTexAll();

	UShort width = pImage->GetBound(0);
	UShort height = pImage->GetBound(1);
	UChar usesMipmaps = pImage->HasMipmaps() ? GX_TRUE : GX_FALSE;
	GXInitTexObj(&pResource->TexObj, pResource->Image, width, height,
		msImageFormat[format], msTexWrapMode[pTexture->GetWrapType(0)],
		msTexWrapMode[pTexture->GetWrapType(1)], usesMipmaps);
}

//----------------------------------------------------------------------------
void GXRenderer::OnReleaseTexture(ResourceIdentifier* pID)
{
	TextureID* pResource = static_cast<TextureID*>(pID);
	free(pResource->Image);	// allocated using memalign, not using new
	WIRE_DELETE pResource;
}

//----------------------------------------------------------------------------
void GXRenderer::ConvertRGBA8888ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					*pDst++ = *(pSrc + offset*4 + 3);
					*pDst++ = *(pSrc + offset*4);
				}
			}

			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UChar* pSrcTemp = pSrc + offset*4 + 1;
					*pDst++ = *pSrcTemp++;
					*pDst++ = *pSrcTemp;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void GXRenderer::ConvertRGB888ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					*pDst++ = 0xFF;
					*pDst++ = *(pSrc + offset*3);
				}
			}

			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UChar* pSrcTemp = pSrc + offset*3 + 1;

					*pDst++ = *pSrcTemp++;
					*pDst++ = *pSrcTemp;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void GXRenderer::ConvertRGB565ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					*pDst++ = *(pSrc+offset*2);
					*pDst++ = *(pSrc+offset*2 + 1);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void GXRenderer::ConvertRGBA4444ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UShort texel = static_cast<UShort>(*(pSrc + offset*2));
					texel = texel << 8;
					texel |= static_cast<UShort>(*(pSrc + offset*2 + 1));
					UShort texelRGB = (texel & 0xFFF0) >> 4;
					UShort texelA = (texel & 0x0e) << 11;
					texel = texelRGB | texelA;
					*pDst++ = static_cast<UChar>(texel >> 8);
					*pDst++ = static_cast<UChar>(texel);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
UInt GXRenderer::GetTotalImageMemory(const Image* pImage, const UInt bpp)
	const
{
	UInt totalMemory = 0;
	for (UInt mipLevel = 0; mipLevel < pImage->GetMipmapCount(); mipLevel++)
	{
		UInt mipMemoryUsed = pImage->GetQuantity(mipLevel) * bpp;
		mipMemoryUsed = mipMemoryUsed < 32 ? 32 : mipMemoryUsed;
		totalMemory += mipMemoryUsed;
	}

	return totalMemory;
}

//----------------------------------------------------------------------------
void GXRenderer::GetTileCount(UInt& rTilesYCount, UShort& rHeight,
	UInt& rTilesXCount, UShort& rWidth)
{
	rTilesYCount = rHeight / 4;
	rTilesXCount = rWidth / 4;

	if (rHeight < 4)
	{
		rTilesYCount = 1;
	}
	else
	{
		rHeight = 4;
	}

	if (rWidth < 4)
	{
		rTilesXCount = 1;
	}
	else
	{
		rWidth = 4;
	}
}
