#include "WireGXRenderer.h"

#include "WireGeometry.h"
#include "WireGXResources.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
void GXRenderer::CreateDisplayList(VBufferID* pResource, const IndexBuffer&
	rIBuffer)
{
	// Note that the display-list buffer area must be forced out of
	// the CPU cache since it will be written using the write-gather pipe
	const UInt tempDLSize = 65536;
	void* pTempDL = memalign(32, tempDLSize);
	DCInvalidateRange(pTempDL, tempDLSize);

	GXBeginDisplayList(pTempDL, tempDLSize);
	Draw(pResource, rIBuffer);
	pResource->DisplayListSize = GXEndDisplayList();

	pResource->DisplayList = memalign(32, pResource->DisplayListSize);
	System::Memcpy(pResource->DisplayList, pResource->DisplayListSize,
		pTempDL, pResource->DisplayListSize);

	DCFlushRange(pResource->DisplayList, pResource->DisplayListSize);
}

//----------------------------------------------------------------------------
void GXRenderer::OnEnableVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	const TArray<VBufferID::VertexElement>& rElements = *(pResource->
		Elements);

	// TODO: add GX texture handling
	for (UInt i = 0; i < 2/*rElements.GetQuantity()*/; i++)
	{
		GXSetVtxDesc(rElements[i].Attr, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, rElements[i].Attr, rElements[i].CompCnt,
			rElements[i].CompType, 0);
		GXSetArray(rElements[i].Attr, rElements[i].Data, rElements[i].Stride);
	}

	if (!pResource->DisplayList)
	{
		CreateDisplayList(pResource, *(mpGeometry->IBuffer));
	}

	mCurrentVBuffer = pResource;
}
