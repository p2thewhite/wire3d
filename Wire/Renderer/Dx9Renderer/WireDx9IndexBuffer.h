#pragma once
#ifndef WIREDX9INDEXBUFFER_H
#define WIREDX9INDEXBUFFER_H

#include "WireBuffer.h"
#include "WireTypes.h"

struct IDirect3DIndexBuffer9;

namespace Wire
{

class Renderer;
class IndexBuffer;

class PdrIndexBuffer
{
public:
	PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer* pIndexBuffer);
	~PdrIndexBuffer();

	void Enable(Renderer* pRenderer, const IndexBuffer* pIndexBuffer);
	void Disable(Renderer* pRenderer);
	void* Lock(Buffer::LockingMode mode);
	void Unlock();

private:
	IDirect3DIndexBuffer9* mpBuffer;
};

}

#endif
