#include "WireGXRenderer.h"

#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
void GXRenderer::OnLoadVBuffer(ResourceIdentifier*& rID,
	VertexBuffer* pVBuffer)
{
	VBufferID* pResource = WIRE_NEW VBufferID;
	rID = pResource;

	const VertexAttributes& rIAttr = pVBuffer->GetAttributes();

	pResource->Elements = WIRE_NEW TArray<VBufferID::VertexElement>(2);

	VBufferID::VertexElement element;

	UInt channels = rIAttr.GetPositionChannels();
	if (channels > 0)
	{
 		element.Data = memalign(32, channels * sizeof(Float) * pVBuffer->
 			GetVertexQuantity());
 		element.Attr = GX_VA_POS;
 		element.CompCnt = GX_POS_XYZ;
 		element.CompType = GX_F32;
 		pResource->Elements->Append(element);
	}

	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
		{
 			element.Data = memalign(32, sizeof(UInt) * pVBuffer->
 				GetVertexQuantity());
 			element.Attr = GX_VA_CLR0;
 			element.CompCnt = GX_CLR_RGBA;
 			element.CompType = GX_RGBA8;
 			pResource->Elements->Append(element);
		}
	}

	WIRE_ASSERT(pResource->Elements->GetQuantity() > 0);
	Convert(pVBuffer, pResource);
}

//----------------------------------------------------------------------------
void GXRenderer::Convert(const VertexBuffer* pSrc, VBufferID* pResource)
{
	const VertexAttributes& rIAttr = pSrc->GetAttributes();
	TArray<VBufferID::VertexElement>& rElements = *(pResource->Elements);

	UInt vertexOffset = 0;
	UInt colorOffset = 0;

	for (UInt i = 0; i < pSrc->GetVertexQuantity(); i++)
	{
		UInt index = 0;

		if (rIAttr.GetPositionChannels() > 0)
		{
			const Float* pPosition = pSrc->GetPosition(i);
			Float* pDst = static_cast<Float*>(rElements[index++].Data);

			for (UInt k = 0; k < rIAttr.GetPositionChannels(); k++)
			{
				pDst[vertexOffset++] = pPosition[k];
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
 				pDst[colorOffset++] = color;
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

	WIRE_DELETE pResource->Elements;
	WIRE_DELETE pResource;
}
