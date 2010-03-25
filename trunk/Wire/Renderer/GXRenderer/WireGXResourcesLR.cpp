#include "WireGXRenderer.h"

#include "WireGeometry.h"
#include "WireGXResources.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
void GXRenderer::OnLoadVBuffer(ResourceIdentifier*& rID,
	VertexBuffer* pVBuffer)
{
	VBufferID* pResource = WIRE_NEW VBufferID;
	pResource->Elements = WIRE_NEW TArray<VBufferID::VertexElement>(2);
	rID = pResource;

	TArray<VBufferID::VertexElement>& rElements = *(pResource->Elements);
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

	WIRE_ASSERT(pResource->Elements->GetQuantity() > 0);
	Convert(pVBuffer, pResource);

 	for (UInt i = 0; i < rElements.GetQuantity(); i++)
 	{
 		// TODO: use DCStoreRangeNoSync to accumulate stores, then PPCSync()
 		DCStoreRange(rElements[i].Data, rElements[i].Size);
 	}

	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();

	pResource->DisplayList = NULL;
	pResource->DisplayListSize = 0;
}

//----------------------------------------------------------------------------
void GXRenderer::Convert(const VertexBuffer* pSrc, VBufferID* pResource)
{
	const VertexAttributes& rIAttr = pSrc->GetAttributes();
	TArray<VBufferID::VertexElement>& rElements = *(pResource->Elements);

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
				UInt color = 0xffffffff;
				for (UInt k = 0; k < rIAttr.GetColorChannels(unit); k++)
				{
					color = color << 8;
					color |= static_cast<UChar>(pColor[k] * 255.0F);
				}

				if (rIAttr.GetColorChannels(unit) == 3)
				{
					color = color << 8;
					color |= 0xff;
				}

				UInt* pDst = static_cast<UInt*>(rElements[index++].Data);
 				pDst[i] = color;
			}
		}

		UInt tChannelQuantity = rIAttr.GetTCoordChannelQuantity();
		for (UInt unit = 0; unit < tChannelQuantity; unit++)
		{
			if (rIAttr.GetColorChannels(unit) > 0)
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
	TArray<VBufferID::VertexElement>& rElements = *(pResource->Elements);
	for (UInt i = 0; i < rElements.GetQuantity(); i++)
	{
		free(rElements[i].Data);	// allocated using memalign, not using new
	}

	free(pResource->DisplayList);	// allocated using memalign, not using new
	WIRE_DELETE pResource->Elements;
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
}

//----------------------------------------------------------------------------
void GXRenderer::OnReleaseTexture(ResourceIdentifier* pID)
{
	TextureID* pResource = static_cast<TextureID*>(pID);
	WIRE_DELETE pResource;
}
