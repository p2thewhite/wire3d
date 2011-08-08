// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireLight.h"
#include "WireGXRendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetLight(const Light* pLight, UInt unit)
{
	WIRE_ASSERT(unit < PdrRendererData::MAXLIGHTS);

	Bool useLight = pLight ? pLight->Enabled : false;
	if (!useLight)
	{
		GXSetNumChans(1);
		mpData->LightsMask &= ~(1<<unit);
		GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX,
			mpData->LightsMask, GX_DF_NONE, GX_AF_NONE);
		return;
	}

	GXLightObj gxLight;
	const ColorRGB& rCol = pLight->Color;
	const GXColor diffuse = { rCol.R()*255, rCol.G()*255, rCol.B()*255,
		255 };

	switch (pLight->Type)
	{
	case Light::LT_POINT:
	{
		Light* pL = const_cast<Light*>(pLight);
		Vec* pLightPos = reinterpret_cast<Vec*>(&pL->Position);
		Vec lightViewPos;
		MTXMultVec(mpData->ViewMatrix, pLightPos, &lightViewPos);
		GXInitLightPosv(&gxLight, &lightViewPos);
		GXInitLightSpot(&gxLight, 0.0F, GX_SP_OFF);
		GXInitLightDistAttn(&gxLight, 20, 1, GX_DA_OFF);
		break;
	}

	case Light::LT_DIRECTIONAL:
	{
		Light* pL = const_cast<Light*>(pLight);
		Vec lightDir = *(reinterpret_cast<Vec*>(&pL->Direction));
		static Vec s_zero = {0, 0, 0};
		VECSubtract(&s_zero, &lightDir, &lightDir);
		lightDir.x = lightDir.x * 1000000;
		lightDir.y = lightDir.y * 1000000;
		lightDir.z = lightDir.z * 1000000;
		MTXMultVec(mpData->ViewMatrix, &lightDir, &lightDir);
		GXInitLightPosv(&gxLight, &lightDir);
		GXInitLightSpot(&gxLight, 0.0F, GX_SP_OFF);
		GXInitLightDistAttn(&gxLight, 20, 1, GX_DA_OFF);
		break;
	}

	case Light::LT_SPOT:
	{
		Light* pL = const_cast<Light*>(pLight);

		Vec* pLightDir = reinterpret_cast<Vec*>(&pL->Direction);
		Vec lightViewDir;
		MTXMultVecSR(mpData->ViewMatrix, pLightDir, &lightViewDir);
		GXInitLightDirv(&gxLight, &lightViewDir);

		Vec* pLightPos = reinterpret_cast<Vec*>(&pL->Position);
		Vec lightViewPos;
		MTXMultVec(mpData->ViewMatrix, pLightPos, &lightViewPos);
		GXInitLightPosv(&gxLight, &lightViewPos);
		
		Float angle = pL->Angle * MathF::RAD_TO_DEG * 0.5F;
		GXInitLightSpot(&gxLight, angle, GX_SP_COS2);
		GXInitLightDistAttn(&gxLight, 20, 1, GX_DA_OFF);
		break;
	}

	default:
		WIRE_ASSERT(false);
	}

// 		Light* pL = const_cast<Light*>(pLight);
// 		Vec* pLightDir = reinterpret_cast<Vec*>(&pL->Direction);
// 		Vec lightViewDir;
// 		MTXMultVecSR(mpData->ViewMatrix, pLightDir, &lightViewDir);
// 		GXInitSpecularDirv(&gxLight, &lightViewDir);
// 		GXInitLightShininess(&gxLight, 4);
//		GXInitLightSpot(&gxLight, 0.0F, GX_SP_OFF);

	GXInitLightColor(&gxLight, diffuse);
	GXLoadLightObjImm(&gxLight, 1 << unit);

	GXSetNumChans(1); // number of color channels
	mpData->LightsMask |= 1<<unit;
	GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, mpData->
		LightsMask, GX_DF_CLAMP, GX_AF_SPOT);

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
	GXSetNumChans(1);

	mpData->LightsMask = 0;
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0,
		GX_DF_NONE, GX_AF_NONE);
}
