#pragma once
#ifndef WIREGXVERTEXBUFFER_H
#define WIREGXVERTEXBUFFER_H

#include "WireTArray.h"
#include "WireTypes.h"

namespace Wire
{

class PdrIndexBuffer;
class Renderer;
class VertexBuffer;

class PdrVertexBuffer
{
public:
	struct VertexElement
	{
		void* Data;
		UInt Size;
		UChar Stride;
		UChar Attr;
		UChar CompCnt;
		UChar CompType;
	};

	struct DisplayList
	{
		void* DL;
		UInt DLSize;
		PdrIndexBuffer* RegisteredIBuffer;
	};

	PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	void Disable(Renderer* pRenderer);

	TArray<DisplayList>& GetDisplayLists() { return mDisplayLists; }
	const TArray<VertexElement>& GetVertexElements() const
		{ return mElements; }

private:
	TArray<DisplayList> mDisplayLists;
	TArray<VertexElement> mElements;

};

}

#endif
