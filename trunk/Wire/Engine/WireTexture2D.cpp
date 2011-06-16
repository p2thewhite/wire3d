// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireTexture2D.h"

#include "WireImage2D.h"
#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Texture2D, Buffer);

//----------------------------------------------------------------------------
Texture2D::Texture2D(Image2D* pImage, UsageType usage)
	:
	Buffer(usage),
	mspImage(pImage),
	mFilterType(FT_LINEAR),
	mAnisotropy(1.0F)
{
	if (pImage->HasMipmaps())
	{
		mFilterType = FT_LINEAR_LINEAR;
	}

	mWarpType[0] = WT_CLAMP;
	mWarpType[1] = WT_CLAMP;
}

//----------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	// Inform all renderers using this texture that it is being destroyed.
	// This allows the renderer to free up any associated resources.
    Renderer::UnbindAll(this);
}
