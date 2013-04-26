// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRERENDERTARGET_H
#define WIRERENDERTARGET_H

#include "WireImage2D.h"

namespace Wire
{

class RenderTarget : public Object
{
	WIRE_DECLARE_RTTI;

public:
	RenderTarget(UInt targetQuantity, Image2D::FormatMode format, UInt width,
		UInt height, Bool hasMipmaps, Bool hasDepthStencil);
	virtual ~RenderTarget();

	inline UInt GetTargetQuantity() const;
	inline Image2D::FormatMode GetFormat() const;
	inline UInt GetWidth() const;
	inline UInt GetHeight() const;
	inline Bool HasMipmaps() const;
	inline Bool HasDepthStencil() const;

private:
	UInt mTargetQuantity;
	Image2D::FormatMode mFormat;
	UInt mWidth;
	UInt mHeight;
	Bool mHasMipmaps;
	Bool mHasDepthStencil;
};

typedef Pointer<RenderTarget> RenderTargetPtr;
#include "WireRenderTarget.inl"

}

#endif
