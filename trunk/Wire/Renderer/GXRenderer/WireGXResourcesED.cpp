#include "WireGXRenderer.h"

#include "WireGeometry.h"
#include "WireGXResources.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
void GXRenderer::CreateDisplayList(VBufferID* pResource, const IndexBuffer&
	rIBuffer)
{
	VBufferID::DisplayList DL;

	// Note that the display-list buffer area must be forced out of
	// the CPU cache since it will be written using the write-gather pipe
	const UInt tempDLSize = 65536;
	void* pTempDL = memalign(32, tempDLSize);
	DCInvalidateRange(pTempDL, tempDLSize);

	GXBeginDisplayList(pTempDL, tempDLSize);
	Draw(pResource, rIBuffer);

	DL.DLSize = GXEndDisplayList();
	DL.DL = memalign(32, DL.DLSize);
	System::Memcpy(DL.DL, DL.DLSize, pTempDL, DL.DLSize);
	free(pTempDL);

	DCFlushRange(DL.DL, DL.DLSize);

	DL.RegisteredIBuffer = mpIBufferID;
	mpIBufferID->RegisteredVBuffers.Append(pResource);
	pResource->DisplayLists.Append(DL);
}

//----------------------------------------------------------------------------
void GXRenderer::OnEnableVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	const TArray<VBufferID::VertexElement>& rElements = pResource->Elements;

	// TODO: add GX texture handling
	for (UInt i = 0; i < 2/*rElements.GetQuantity()*/; i++)
	{
		GXSetVtxDesc(rElements[i].Attr, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, rElements[i].Attr, rElements[i].CompCnt,
			rElements[i].CompType, 0);
		GXSetArray(rElements[i].Attr, rElements[i].Data, rElements[i].Stride);
	}

	// Check if there is a displaylist for this Vertex- and Indexbuffer
	// combination.
	TArray<VBufferID::DisplayList>& rDisplayLists = pResource->DisplayLists;
	Bool foundDL = false;
	for (UInt i = 0; i < rDisplayLists.GetQuantity(); i++)
	{
		if (rDisplayLists[i].RegisteredIBuffer == mpIBufferID)
		{
			foundDL = true;
		}
	}

	if (!foundDL)
	{
		CreateDisplayList(pResource, *(mpGeometry->IBuffer));
	}

	mpVBufferID = pResource;
}

//----------------------------------------------------------------------------
void GXRenderer::OnEnableIBuffer(ResourceIdentifier* pID)
{
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	mpIBufferID = pResource;
}
