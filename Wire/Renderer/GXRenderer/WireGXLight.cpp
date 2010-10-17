#include "WireRenderer.h"

#include "WireLight.h"
#include "WireGXRendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetLight(const Light* pLight, UInt unit)
{
	if (!pLight)
	{
		GXSetNumChans(1);
		GXSetChanCtrl(
			GX_COLOR0A0,
			GX_DISABLE,					// disable channel
			GX_SRC_VTX,					// amb source
			GX_SRC_VTX,					// mat source
			1 << unit /*GX_LIGHT0*/,	// light mask
			GX_DF_NONE,					// diffuse function
			GX_AF_NONE);
		return;
	}

	GXLightObj gxLight;
	const ColorRGB& rCol = pLight->Color;
	const GXColor diffuse = { rCol.R()*255, rCol.G()*255, rCol.B()*255, 255 };

	Vector lightWorldPos = { 0, 0, 0 };
	Vector lightViewPos;
	MTXMultVec(mpData->ViewMatrix, &lightWorldPos, &lightViewPos);
	GXInitLightPos(&gxLight, lightViewPos.x, lightViewPos.y, lightViewPos.z);
	GXInitLightColor(&gxLight, diffuse);
	GXLoadLightObjImm(&gxLight, 1 << unit /*GX_LIGHT0*/);

	GXSetNumChans(1); // number of color channels
	GXSetChanCtrl(
		GX_COLOR0A0,
		GX_ENABLE,						// enable channel
		GX_SRC_REG,						// amb source
		GX_SRC_REG,						// mat source
		1 << unit /*GX_LIGHT0*/,		// light mask
		GX_DF_CLAMP,					// diffuse function
		GX_AF_NONE);

	const ColorRGB& rAmb = pLight->Ambient;
	const GXColor ambient = { rAmb.R()*255, rAmb.G()*255, rAmb.B()*255, 255};
	GXSetChanAmbColor(GX_COLOR0A0, ambient);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xFF, 0xFF, 0, 0xFF}/*mpData->Material*/);
}

//----------------------------------------------------------------------------
void Renderer::EnableLighting()
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
}

//----------------------------------------------------------------------------
void Renderer::DisableLighting()
{

}
