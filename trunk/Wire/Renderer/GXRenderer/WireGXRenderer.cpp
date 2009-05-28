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

	GXSetCullMode(GX_CULL_BACK);

	const VertexBuffer* pVBuffer = mpGeometry->VBuffer;

	GXBegin(GX_QUADS, GX_VTXFMT0, pVBuffer->GetQuantity());

	for (unsigned int i = 0; i < pVBuffer->GetQuantity(); i++)
	{
		const Vector3f& rVertex = pVBuffer->Position3(i);
		const Vector3f& rColor = pVBuffer->Color3(i);
		GXPosition3f32(rVertex.X(), rVertex.Y(), rVertex.Z());
		GX_Color3f32(rColor.X(), rColor.Y(), rColor.Z());
	}

	GXEnd();
}
