// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREDX9TEXTURE2D_H
#define WIREDX9TEXTURE2D_H

#include "WireImage2D.h"
#include "WireTypes.h"

struct IDirect3DTexture9;

namespace Wire
{

class Renderer;
class Texture2D;

class PdrTexture2D
{
public:
	PdrTexture2D(Renderer* pRenderer, const Image2D* pImage);
	~PdrTexture2D();

	void Enable(Renderer* pRenderer, const Texture2D* pTexture, UInt unit);
	void Disable(Renderer* pRenderer, UInt unit);
	void* Lock(Buffer::LockingMode mode, UInt level = 0);
	void Unlock(UInt level = 0);

	inline UInt GetBufferSize() const { return mBufferSize; }

private:
	friend class PdrRenderTarget;
	// RenderTarget texture/depth stencil
	PdrTexture2D(Renderer* pRenderer, Image2D::FormatMode format, UInt width,
		UInt height, Bool autoGenerateMipMaps);
	void Enable(Renderer* pRenderer, UInt unit);

	IDirect3DTexture9* mpBuffer;
	UInt mBufferSize;
};

}

#endif
