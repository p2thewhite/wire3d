#include "WireGXRenderer.h"

using namespace Wire;

//-------------------------------------------------------------------------
GXRenderer::GXRenderer()
{
}

//-------------------------------------------------------------------------
GXRenderer::~GXRenderer()
{
}

//-------------------------------------------------------------------------
void GXRenderer::SetClearColor(const ColorRGBA& rClearColor)
{
	Parent::SetClearColor(rClearColor);

	mGXClearColor.r = static_cast<UChar>(rClearColor.R() * 255.0f);
	mGXClearColor.g = static_cast<UChar>(rClearColor.G() * 255.0f);
	mGXClearColor.b = static_cast<UChar>(rClearColor.B() * 255.0f);
	mGXClearColor.a = static_cast<UChar>(rClearColor.A() * 255.0f);
	GXSetCopyClear(mGXClearColor, GX_MAX_Z24);
}

//-------------------------------------------------------------------------
void GXRenderer::DrawElements()
{
	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	// setup the vertex attribute table
	// describes the data
	// args: vat location 0-7, type of data, data format, size, scale
	// so for ex. in the first call we are sending position data with
	// 3 values X,Y,Z of size F32. scale sets the number of fractional
	// bits for non float data.
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);

	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	Matrix34f model;
	mpGeometry->Local.GetTransformation(model);
	// load the modelview matrix into matrix memory
	GXLoadPosMtxImm((*View) * model, GX_PNMTX0);

	const VertexBuffer* pVBuffer = mpGeometry->VBuffer;

	UChar gxPrimitive = GX_TRIANGLES;
	if (mpGeometry->Type == Geometry::GT_QUADMESH)
	{
		gxPrimitive = GX_QUADS;
	}

	GXBegin(gxPrimitive, GX_VTXFMT0, pVBuffer->GetQuantity());

	for (UInt i = 0; i < pVBuffer->GetQuantity(); i++)
	{
		const Vector3f& rVertex = pVBuffer->Position3(i);
		const Vector3f& rColor = pVBuffer->Color3(i);
		GXPosition3f32(rVertex.X(), rVertex.Y(), rVertex.Z());
		GX_Color3f32(rColor.X(), rColor.Y(), rColor.Z());
	}

	GXEnd();
}
