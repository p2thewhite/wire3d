// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREDX9INDEXBUFFER_H
#define WIREDX9INDEXBUFFER_H

#include "WireBuffer.h"
#include "WireDx9RendererData.h"
#include "WireTypes.h"
#include "WireRenderer.h"

struct IDirect3DIndexBuffer9;

namespace Wire
{

class Renderer;
class IndexBuffer;

class PdrIndexBuffer
{
public:
	PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer* pIndexBuffer);
	PdrIndexBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType usage);

	~PdrIndexBuffer();

	inline void Enable(Renderer* pRenderer);
	inline void Disable(Renderer* pRenderer);

	void* Lock(Buffer::LockingMode mode);
	void Unlock();

	void Update(const IndexBuffer* pIndexBuffer);
	void Copy(const IndexBuffer* pIndexBuffer, void* pBuffer, UInt offset);

	inline UInt GetBufferSize() const;
	inline UInt GetIndexSize() const;

private:
	void CreateBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType
		usage);

	IDirect3DIndexBuffer9* mpBuffer;
	UInt mBufferSize;
	UInt mIndexSize;
};

#include "WireDx9IndexBuffer.inl"

}

#endif
