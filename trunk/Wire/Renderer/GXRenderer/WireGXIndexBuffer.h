#pragma once
#ifndef WIREGXINDEXBUFFER_H
#define WIREGXINDEXBUFFER_H

#include "WireTArray.h"
#include "WireTypes.h"

namespace Wire
{

class Renderer;
class IndexBuffer;
class PdrVertexBuffer;

class PdrIndexBuffer
{
public:
	PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer* pIndexBuffer);
	~PdrIndexBuffer();

	void Enable(Renderer* pRenderer, const IndexBuffer* pIndexBuffer);
	void Disable(Renderer* pRenderer);

	TArray<PdrVertexBuffer*>& GetPdrVBuffers() { return mPdrVBuffers; }

private:
	TArray<PdrVertexBuffer*> mPdrVBuffers;
};

}

#endif
