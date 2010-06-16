#pragma once
#ifndef WIREDX9VERTEXBUFFER_H
#define WIREDX9VERTEXBUFFER_H

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

private:
	IDirect3DVertexBuffer9* mpVertexBuffer;
	IDirect3DVertexDeclaration9* mpVertexDeclaration;
	UInt mVertexSize;
};

}

#endif
