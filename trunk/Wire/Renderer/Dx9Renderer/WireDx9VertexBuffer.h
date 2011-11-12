// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREDX9VERTEXBUFFER_H
#define WIREDX9VERTEXBUFFER_H

#include "WireBuffer.h"
#include "WireDx9RendererData.h"
#include "WireRenderer.h"

namespace Wire
{

class VertexAttributes;
class VertexBuffer;

class PdrVertexBuffer
{
public:
	PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	PdrVertexBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType usage);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);

	void* Lock(Buffer::LockingMode mode);
	inline void Unlock();

	void Update(const VertexBuffer* pVertexBuffer);

	inline void SetBuffer(Renderer* pRenderer);
	inline void SetDeclaration(Renderer* pRenderer);

private:
	void CreateBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType
		usage);
	void CreateDeclaration(Renderer* pRenderer, const VertexAttributes&
		rAttributes);
	void Convert(const VertexBuffer* pSrc, Float* pDst);

	IDirect3DVertexBuffer9* mpBuffer;
	IDirect3DVertexDeclaration9* mpDeclaration;
	UInt mVertexSize;
	UInt mVBOSize;
};

#include "WireDx9VertexBuffer.inl"

}

#endif
