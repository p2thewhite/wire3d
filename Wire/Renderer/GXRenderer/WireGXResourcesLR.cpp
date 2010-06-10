#include "WireRenderer.h"

#include "WireGeometry.h"
#include "WireGXRendererData.h"
#include "WireGXResources.h"
#include "WireTexture.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

UChar PdrRendererData::msImageFormat[Image::FM_QUANTITY] =
{
	GX_TF_RGBA8,	// Image::FM_RGB888
	GX_TF_RGBA8,	// Image::FM_RGBA8888
	GX_TF_RGB565,	// Image::FM_RGB565
	GX_TF_RGB5A3,	// Image::FM_RGBA4444
};

//----------------------------------------------------------------------------
void Renderer::OnLoadVBuffer(ResourceIdentifier*& rID,
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
	mpData->Convert(pVBuffer, pResource);

 	for (UInt i = 0; i < rElements.GetQuantity(); i++)
 	{
 		// TODO: use DCStoreRangeNoSync to accumulate stores, then PPCSync()
 		DCStoreRange(rElements[i].Data, rElements[i].Size);
 	}

	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();
}

//----------------------------------------------------------------------------
void Renderer::OnReleaseVBuffer(ResourceIdentifier* pID)
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
void Renderer::OnLoadIBuffer(ResourceIdentifier*& rID, IndexBuffer* pBuffer)
{
	// The texture is encountered the first time. Set up the texture unit
	// in hardware that will manage this texture.
	IBufferID* pResource = WIRE_NEW IBufferID;
	rID = pResource;
}

//----------------------------------------------------------------------------
void Renderer::OnReleaseIBuffer(ResourceIdentifier* pID)
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
void Renderer::OnLoadTexture(ResourceIdentifier*& rID, Texture* pTexture)
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
	UInt totalMemory = mpData->GetTotalImageMemory(pImage, bpp);

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
				mpData->ConvertRGBA8888ToTiles(pSrc, width, height, pDst);
				break;

			case Image::FM_RGB888:
				mpData->ConvertRGB888ToTiles(pSrc, width, height, pDst);
				break;

			case Image::FM_RGB565:
				mpData->ConvertRGB565ToTiles(pSrc, width, height, pDst);
				break;

			case Image::FM_RGBA4444:
				mpData->ConvertRGBA4444ToTiles(pSrc, width, height, pDst);
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
		PdrRendererData::msImageFormat[format],
		PdrRendererData::msTexWrapMode[pTexture->GetWrapType(0)],
		PdrRendererData::msTexWrapMode[pTexture->GetWrapType(1)],
		usesMipmaps);
}

//----------------------------------------------------------------------------
void Renderer::OnReleaseTexture(ResourceIdentifier* pID)
{
	TextureID* pResource = static_cast<TextureID*>(pID);
	free(pResource->Image);	// allocated using memalign, not using new
	WIRE_DELETE pResource;
}
