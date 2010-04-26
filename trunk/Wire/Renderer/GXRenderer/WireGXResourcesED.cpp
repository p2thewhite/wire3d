#include "WireGXRenderer.h"

#include "WireGeometry.h"
#include "WireGXResources.h"
#include "WireTexture.h"
#include <malloc.h>

using namespace Wire;

UChar GXRenderer::msTexMinFilter[Texture::FT_QUANTITY] =
{
	GX_NEAR,			// Texture::FT_NEAREST
	GX_LINEAR,			// Texture::FT_LINEAR
	GX_NEAR_MIP_NEAR,	// Texture::FT_NEAREST_NEAREST
	GX_NEAR_MIP_LIN,	// Texture::FT_NEAREST_LINEAR
	GX_LIN_MIP_NEAR,	// Texture::FT_LINEAR_NEAREST
	GX_LIN_MIP_LIN,		// Texture::FT_LINEAR_LINEAR
};

UChar GXRenderer::msTexWrapMode[Texture::WT_QUANTITY] =
{
	GX_CLAMP,	// Texture::WT_CLAMP
	GX_REPEAT,	// Texture::WT_REPEAT
	GX_MIRROR,	// Texture::WT_MIRRORED_REPEAT
};

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

	GXSetNumTexGens(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
}

//----------------------------------------------------------------------------
void GXRenderer::OnEnableTexture(ResourceIdentifier* pID)
{
	TextureID* pTexID = static_cast<TextureID*>(pID);
	Texture* pTex = pTexID->TextureObject;

	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GXSetNumTexGens(1);

	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

	GXInitTexObjWrapMode(&pTexID->TexObj, msTexWrapMode[pTex->GetWrapType(0)],
		msTexWrapMode[pTex->GetWrapType(1)]);

	Float anisotropy = static_cast<UInt>(pTex->GetAnisotropyValue());
	UChar anisoEnum = GX_ANISO_1;
	UChar doEdgeLod = GX_FALSE;
	if (1.0F < anisotropy && anisotropy <= msMaxAnisotropy)
	{
		anisoEnum = (anisotropy - 3.0F) > 0.0F ? GX_ANISO_4 : GX_ANISO_2;
		doEdgeLod = GX_TRUE;
	}

	UChar magFilter = pTex->GetFilterType() == Texture::FT_NEAREST ?
		GX_NEAR : GX_LINEAR;
  	GXInitTexObjLOD(&pTexID->TexObj, msTexMinFilter[pTex->GetFilterType()],
  		magFilter, 0, 10.0F, 0, GX_FALSE, doEdgeLod, anisoEnum);

	// set texture
	GXLoadTexObj(&pTexID->TexObj, GX_TEXMAP0);
}
