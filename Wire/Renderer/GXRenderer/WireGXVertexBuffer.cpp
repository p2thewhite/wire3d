#include "WireGXVertexBuffer.h"

#include "WireGeometry.h"
#include "WireGXIndexBuffer.h"
#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer*
	pVertexBuffer)
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

	WIRE_ASSERT(mElements.GetQuantity() > 0);
	pRenderer->GetRendererData()->Convert(pVertexBuffer, mElements);

	for (UInt i = 0; i < mElements.GetQuantity(); i++)
	{
		// TODO: use DCStoreRangeNoSync to accumulate stores, then PPCSync()
		DCStoreRange(mElements[i].Data, mElements[i].Size);
	}

	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();
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
			mDisplayLists[i].RegisteredIBuffer->mVBuffers;

		for (UInt j = 0; j < rVBuffers.GetQuantity(); j++)
		{
			if (rVBuffers[j] == this)
			{
				rVBuffers.Remove(j);
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
		if (mDisplayLists[i].RegisteredIBuffer == rData.mpPdrIBuffer)
		{
			foundDL = true;
			break;
		}
	}

	if (!foundDL)
	{
		rData.CreateDisplayList(this, *(pRenderer->mpGeometry->IBuffer));
	}

	rData.mpPdrVBuffer = this;
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable(Renderer* pRenderer)
{
	// Nothing to do.
}
