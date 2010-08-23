#include "WireGXIndexBuffer.h"

#include "WireIndexBuffer.h"
#include "WireGXRendererData.h"
#include "WireGXVertexBuffer.h"
#include "WireRenderer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer*
	pIndexBuffer)
{
	// Nothing to do.
}

//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer()
{
	for (UInt i = 0; i < mPdrVBuffers.GetQuantity(); i++)
	{
		TArray<PdrVertexBuffer::DisplayList>& rDisplayLists =
			mPdrVBuffers[i]->GetDisplayLists();

		for (UInt j = 0; j < rDisplayLists.GetQuantity(); j++)
		{
			if (rDisplayLists[j].RegisteredIBuffer == this)
			{
				free(rDisplayLists[i].DL);	// allocated using memalign
				rDisplayLists.RemoveAt(j);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable(Renderer* pRenderer, const IndexBuffer*
	pIndexBuffer)
{
	PdrRendererData* pData = pRenderer->GetRendererData();
	pData->PdrIBuffer = this;
	pData->IBuffer = const_cast<IndexBuffer*>(pIndexBuffer);

	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable(Renderer* pRenderer)
{
	// Nothing to do.
}
