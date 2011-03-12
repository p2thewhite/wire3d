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

	void Enable(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
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
