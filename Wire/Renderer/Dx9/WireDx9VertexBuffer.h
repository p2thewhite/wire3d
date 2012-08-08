// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
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

	inline void Enable(Renderer* pRenderer);
	inline void Disable(Renderer* pRenderer);

	inline void* Lock(Buffer::LockingMode mode);
	inline void Unlock();

	void Update(const VertexBuffer* pVertexBuffer);
 	void Update(const VertexBuffer* pVertexBuffer, UInt count,
 		UInt offset = 0);

	inline UInt GetBufferSize() const;

	inline void SetDeclaration(Renderer* pRenderer);
	void CreateDeclaration(Renderer* pRenderer, const VertexAttributes&
		rAttributes);
	inline UInt GetVertexSize() const;

private:
	void CreateBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType
		usage);
	inline void SetBuffer(Renderer* pRenderer, UInt vertexSize);

	IDirect3DVertexBuffer9* mpBuffer;
	UInt mBufferSize;

	IDirect3DVertexDeclaration9* mpDeclaration;
	UInt mVertexSize;
};

#include "WireDx9VertexBuffer.inl"

}

#endif
