#include "WireRenderer.h"

#include "WireLight.h"
#include "WireGXRendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetLight(const Light* pLight, UInt unit)
{
	WIRE_ASSERT(unit < PdrRendererData::MaxLights)

	if (!pLight)
	{
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX,
			1 << unit, GX_DF_NONE, GX_AF_NONE);
	}

	GXLightObj gxLight;
	const ColorRGB& rCol = pLight->Color;
	const GXColor diffuse = { rCol.R()*255, rCol.G()*255, rCol.B()*255,
		255 };

	Vector lightWorldPos = { 0, 0, 0 };
	Vector lightViewPos;
	MTXMultVec(mpData->ViewMatrix, &lightWorldPos, &lightViewPos);
	GXInitLightPos(&gxLight, lightViewPos.x, lightViewPos.y, lightViewPos.z);
	GXInitLightColor(&gxLight, diffuse);
	GXLoadLightObjImm(&gxLight, 1 << unit);

	GXSetNumChans(1); // number of color channels
	GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, 1 << unit,
		GX_DF_CLAMP, GX_AF_NONE);

	const ColorRGB& rAmb = pLight->Ambient;
	const GXColor amb = { rAmb.R()*255, rAmb.G()*255, rAmb.B()*255, 255};
	GXSetChanAmbColor(GX_COLOR0A0, amb);
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
