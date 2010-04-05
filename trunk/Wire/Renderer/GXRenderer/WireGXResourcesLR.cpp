#include "WireGXRenderer.h"

#include "WireGeometry.h"
#include "WireGXResources.h"
#include "WireTexture.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

UChar GXRenderer::msImageFormat[Image::FM_QUANTITY] =
{
	GX_TF_RGB565,      // Image::FM_RGB888
	GX_TF_RGB5A3,      // Image::FM_RGBA8888
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

	// Copy the image data from system memory to video memory.
	const Image* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	UInt quantity = pImage->GetQuantity();
	UInt bpp = 2;
	UChar* pSrc = pImage->GetData();
	pResource->Image = memalign(32, quantity * bpp);
	UChar* pDst = static_cast<UChar*>(pResource->Image);
	WIRE_ASSERT(pDst);
	Image::FormatMode format = pImage->GetFormat();
	UShort width = pImage->GetBound(0);
	UShort height = pImage->GetBound(1);

	if (pSrc)
	{
		switch (format)
		{
		case Image::FM_RGBA8888:
			for (UInt i = 0; i < quantity; i++)
			{
// 				*pDst++ = *pSrc++;
// 				*pDst++ = *pSrc++;
// 				*pDst++ = *pSrc++;
// 				*pDst++ = *pSrc++;
			}

			break;

		case Image::FM_RGB888:	// GX_TF_RGB565
			for (UInt ty = 0; ty < height / 4; ty++)
			{
				for (UInt tx = 0; tx < width / 4; tx++)
				{
					for (UInt y = 0; y < 4; y++)
					{
						for (UInt x = 0; x < 4; x++)
						{
							UInt offset = (ty<<2)*width+(tx<<2)+y*width+x;
							UShort rgb565 = RGB888toRGB565(pSrc+offset*3);

							*pDst++ = static_cast<UChar>(rgb565 >> 8);
							*pDst++ = static_cast<UChar>(rgb565);
						}
					}
				}
			}

			break;

		default:
			break;
		}
	}

	DCStoreRange(pResource->Image, quantity * bpp);
	GXInvalidateTexAll();

 	GXInitTexObj(&pResource->TexObj, pResource->Image, width, height,
		msImageFormat[format], msTexWrapMode[pTexture->GetWrapType(0)],
		msTexWrapMode[pTexture->GetWrapType(1)], GX_FALSE);
}

//----------------------------------------------------------------------------
void GXRenderer::OnReleaseTexture(ResourceIdentifier* pID)
{
	TextureID* pResource = static_cast<TextureID*>(pID);
	free(pResource->Image);	// allocated using memalign, not using new
	WIRE_DELETE pResource;
}
