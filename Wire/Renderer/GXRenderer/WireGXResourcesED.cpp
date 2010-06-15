#include "WireRenderer.h"

#include "WireGeometry.h"
#include "WireGXRendererData.h"
#include "WireGXResources.h"
#include "WireTexture2D.h"
#include <malloc.h>

using namespace Wire;

UChar PdrRendererData::msTexMinFilter[Texture2D::FT_QUANTITY] =
{
	GX_NEAR,			// Texture2D::FT_NEAREST
	GX_LINEAR,			// Texture2D::FT_LINEAR
	GX_NEAR_MIP_NEAR,	// Texture2D::FT_NEAREST_NEAREST
	GX_NEAR_MIP_LIN,	// Texture2D::FT_NEAREST_LINEAR
	GX_LIN_MIP_NEAR,	// Texture2D::FT_LINEAR_NEAREST
	GX_LIN_MIP_LIN,		// Texture2D::FT_LINEAR_LINEAR
};

UChar PdrRendererData::msTexWrapMode[Texture2D::WT_QUANTITY] =
{
	GX_CLAMP,	// Texture2D::WT_CLAMP
	GX_REPEAT,	// Texture2D::WT_REPEAT
	GX_MIRROR,	// Texture2D::WT_MIRRORED_REPEAT
};

//----------------------------------------------------------------------------
void Renderer::OnEnableVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	const TArray<VBufferID::VertexElement>& rElements = pResource->Elements;

	for (UInt i = 0; i < rElements.GetQuantity(); i++)
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
		if (rDisplayLists[i].RegisteredIBuffer == mpData->mpIBufferID)
		{
			foundDL = true;
		}
	}

	if (!foundDL)
	{
		mpData->CreateDisplayList(pResource, *(mpGeometry->IBuffer));
	}

	mpData->mpVBufferID = pResource;
}

//----------------------------------------------------------------------------
void Renderer::OnDisableVBuffer(ResourceIdentifier* pID)
{
}

//----------------------------------------------------------------------------
void Renderer::OnEnableIBuffer(ResourceIdentifier* pID)
{
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	mpData->mpIBufferID = pResource;

	GXSetNumTexGens(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
}

//----------------------------------------------------------------------------
void Renderer::OnDisableIBuffer(ResourceIdentifier* pID)
{
}
