#include "WireGXRenderer.h"

#include "WireGXResources.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
void GXRenderer::OnLoadVBuffer(ResourceIdentifier*& rID,
	VertexBuffer* pVBuffer)
{
	VBufferID* pResource = WIRE_NEW VBufferID;
	rID = pResource;

	UInt vertexSize = 0;
	UInt channels = 0;
	const VertexAttributes& rIAttr = pVBuffer->GetAttributes();

	pResource->Elements = WIRE_NEW TArray<VBufferID::VertexElement>(2);

	VBufferID::VertexElement element;

	if (rIAttr.GetPositionChannels() > 0)
	{
		channels = rIAttr.GetPositionChannels();
		vertexSize += channels * sizeof(Float);

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
			vertexSize += sizeof(UInt);
	
 			element.Data = memalign(32, sizeof(UInt) * pVBuffer->
 				GetVertexQuantity());
 			element.Attr = GX_VA_CLR0;
 			element.CompCnt = GX_CLR_RGBA;
 			element.CompType = GX_RGBA8;
 			pResource->Elements->Append(element);
		}
	}

	WIRE_ASSERT(channels > 0);

	pResource->ID = static_cast<Float*>(memalign(32, vertexSize * pVBuffer->
		GetVertexQuantity()));
	pResource->VertexSize = vertexSize;

	Convert(pVBuffer, pResource->ID);
}

//----------------------------------------------------------------------------
void GXRenderer::Convert(const VertexBuffer* pSrc, Float* pDst)
{
	const VertexAttributes& rIAttr = pSrc->GetAttributes();

	for (UInt i = 0; i < pSrc->GetVertexQuantity(); i++)
	{
		if (rIAttr.GetPositionChannels() > 0)
		{
			const Float* pPosition = pSrc->GetPosition(i);
			for (UInt k = 0; k < rIAttr.GetPositionChannels(); k++)
			{
				*pDst++ = pPosition[k];
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

				UInt* pColorDst = reinterpret_cast<UInt*>(pDst);
				*pColorDst++ = color;
				pDst = reinterpret_cast<Float*>(pColorDst);
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
	free(pResource->ID);			// allocated using memalign, not using new
	WIRE_DELETE pResource;
}
