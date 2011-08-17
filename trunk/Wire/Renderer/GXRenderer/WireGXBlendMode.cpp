// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

const UChar PdrRendererData::TEX_BLEND[Material::BM_QUANTITY] =
{
	GX_REPLACE,			// Material::BM_REPLACE
	GX_MODULATE,		// Material::BM_MODULATE
	GX_PASSCLR,			// Material::BM_PASS
	GX_BLEND,			// Material::BM_BLEND
	GX_DECAL			// Material::BM_DECAL
};

//----------------------------------------------------------------------------
void Renderer::SetBlendMode(Material::BlendMode blendMode, UInt unit,
	Bool hasAlpha)
{
	GXSetTevOp(GX_TEVSTAGE0 + unit, PdrRendererData::TEX_BLEND[blendMode]);
}
