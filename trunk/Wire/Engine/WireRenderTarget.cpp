// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderTarget.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, RenderTarget, Object);

//----------------------------------------------------------------------------
RenderTarget::RenderTarget(UInt targetQuantity, Image2D::FormatMode format,
	UInt width, UInt height, Bool hasMipmaps, Bool hasDepthStencil)
	:
	mTargetQuantity(targetQuantity),
	mFormat(format),
	mWidth(width),
	mHeight(height),
	mHasMipmaps(hasMipmaps),
	mHasDepthStencil(hasDepthStencil)
{
	WIRE_ASSERT(targetQuantity > 0 /* Number of targets must be at least 1 */);
}

//----------------------------------------------------------------------------
RenderTarget::~RenderTarget()
{
//	Renderer::UnbindAll(this);
}
