#include "WireGXRenderer.h"

#include "WireGXResources.h"

using namespace Wire;

//----------------------------------------------------------------------------
void GXRenderer::OnEnableVBuffer(ResourceIdentifier* pID)
{
	const VBufferID* pResource = static_cast<const VBufferID*>(pID);

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
}
