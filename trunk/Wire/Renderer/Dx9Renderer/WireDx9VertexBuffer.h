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
#include "WireTypes.h"

struct IDirect3DVertexBuffer9;
struct IDirect3DVertexDeclaration9;

namespace Wire
{

class Renderer;
class VertexBuffer;

class PdrVertexBuffer
{
public:
	PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);
	void* Lock(Buffer::LockingMode mode);
	void Unlock();
	void Update(const VertexBuffer* pVertexBuffer);

private:
	void Convert(const VertexBuffer* pSrc, Float* pDst);

	IDirect3DVertexBuffer9* mpBuffer;
	IDirect3DVertexDeclaration9* mpDeclaration;
	UInt mVertexSize;
};

}

#endif
