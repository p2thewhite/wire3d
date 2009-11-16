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

	if (rIAttr.GetPositionChannels() > 0)
	{
		channels = rIAttr.GetPositionChannels();
		vertexSize += channels * sizeof(Float);
	}

	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
		{
			vertexSize += sizeof(UInt);
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
	WIRE_DELETE pResource->ID;
	WIRE_DELETE pResource;
}
