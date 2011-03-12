#include "WireGXVertexBuffer.h"

#include "WireGeometry.h"
#include "WireGXIndexBuffer.h"
#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer*, const VertexBuffer* pVertexBuffer)
	:
	mHasNormals(false)
{
	VertexElement element;
	UInt vertexCount = pVertexBuffer->GetVertexQuantity();

	const VertexAttributes& rIAttr = pVertexBuffer->GetAttributes();
	UInt channels = rIAttr.GetPositionChannels();
	if (channels > 0)
	{
		element.Stride = channels * sizeof(Float);
		element.Size = element.Stride * vertexCount;
		element.Data = memalign(32, element.Size);
		element.Attr = GX_VA_POS;
		element.CompCnt = GX_POS_XYZ;
		element.CompType = GX_F32;
		mElements.Append(element);
	}

	channels = rIAttr.GetNormalChannels();
	if (channels > 0)
	{
		mHasNormals = true;
		element.Stride = channels * sizeof(Float);
		element.Size = element.Stride * vertexCount;
		element.Data = memalign(32, element.Size);
		element.Attr = GX_VA_NRM;
		element.CompCnt = GX_NRM_XYZ;
		element.CompType = GX_F32;
		mElements.Append(element);
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
			mElements.Append(element);
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
			mElements.Append(element);
		}
	}

	Update(pVertexBuffer);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	for (UInt i = 0; i < mElements.GetQuantity(); i++)
	{
		free(mElements[i].Data);	// allocated using memalign, not using new
	}

	for (UInt i = 0; i < mDisplayLists.GetQuantity(); i++)
	{
		free(mDisplayLists[i].DL);	// allocated using memalign, not using new

		TArray<PdrVertexBuffer*>& rVBuffers =
			mDisplayLists[i].RegisteredIBuffer->GetPdrVBuffers();

		for (UInt j = 0; j < rVBuffers.GetQuantity(); j++)
		{
			if (rVBuffers[j] == this)
			{
				rVBuffers.RemoveAt(j);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer* pRenderer, const VertexBuffer*
	pVertexBuffer)
{
	PdrRendererData& rData = *(pRenderer->GetRendererData());

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();

	for (UInt i = 0; i < mElements.GetQuantity(); i++)
	{
		GXSetVtxDesc(mElements[i].Attr, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, mElements[i].Attr, mElements[i].CompCnt,
			mElements[i].CompType, 0);
		GXSetArray(mElements[i].Attr, mElements[i].Data, mElements[i].Stride);
	}

	// Check if there is a displaylist for this Vertex- and Indexbuffer
	// combination.
	Bool foundDL = false;
	for (UInt i = 0; i < mDisplayLists.GetQuantity(); i++)
	{
		if (mDisplayLists[i].RegisteredIBuffer == rData.PdrIBuffer)
		{
			foundDL = true;
			break;
		}
	}

	if (!foundDL)
	{
		rData.CreateDisplayList(this, *(rData.IBuffer));
	}

	rData.PdrVBuffer = this;
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable(Renderer* pRenderer)
{
	// Nothing to do.
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer)
{
	Convert(pVertexBuffer, mElements);

	for (UInt i = 0; i < mElements.GetQuantity(); i++)
	{
		DCStoreRangeNoSync(mElements[i].Data, mElements[i].Size);
	}

	PPCSync();

	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Convert(const VertexBuffer* pSrc,
	TArray<PdrVertexBuffer::VertexElement>& rElements)
{
	WIRE_ASSERT(rElements.GetQuantity() > 0);
	const VertexAttributes& rIAttr = pSrc->GetAttributes();

	for (UInt i = 0; i < pSrc->GetVertexQuantity(); i++)
	{
		UInt index = 0;

		if (rIAttr.GetPositionChannels() > 0)
		{
			const Float* const pPosition = pSrc->GetPosition(i);
			Float* const pDst = static_cast<Float*>(rElements[index++].Data);
			UInt channelCount = rIAttr.GetPositionChannels();

			for (UInt k = 0; k < channelCount; k++)
			{
				pDst[i*channelCount+k] = pPosition[k];
			}
		}

		if (rIAttr.GetNormalChannels() > 0)
		{
			const Float* const pNormal = pSrc->GetNormal(i);
			Float* const pDst = static_cast<Float*>(rElements[index++].Data);
			UInt channelCount = rIAttr.GetNormalChannels();

			for (UInt k = 0; k < channelCount; k++)
			{
				pDst[i*channelCount+k] = pNormal[k];
			}
		}

		UInt colorChannelQuantity = rIAttr.GetColorChannelQuantity();
		for (UInt unit = 0; unit < colorChannelQuantity; unit++)
		{
			if (rIAttr.GetColorChannels(unit) > 0)
			{
				const Float* const pColor = pSrc->GetColor(i, unit);
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

				UInt* const pDst = static_cast<UInt*>(rElements[index++].
					Data);
				pDst[i] = color;
			}
		}

		UInt tChannelQuantity = rIAttr.GetTCoordChannelQuantity();
		for (UInt unit = 0; unit < tChannelQuantity; unit++)
		{
			if (rIAttr.GetTCoordChannels(unit) > 0)
			{
				const Float* const pUv = pSrc->GetTCoord(i, unit);
				Float* const pDst = static_cast<Float*>(rElements[index++].
					Data);
				UInt channelCount = rIAttr.GetTCoordChannels(unit);

				for (UInt k = 0; k < channelCount; k++)
				{
					pDst[i*channelCount+k] = pUv[k];
				}
			}
		}
	}
}
