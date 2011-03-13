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
		UChar Attr;
		UChar CompCnt;
		UChar CompType;
	};

	struct DisplayList
	{
		void* DL;
		UInt Size;
		PdrIndexBuffer* RegisteredIBuffer;
	};

	PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	void Disable(Renderer* pRenderer);
	void Update(const VertexBuffer* pVertexBuffer);

	TArray<DisplayList>& GetDisplayLists() { return mDisplayLists; }
	const TArray<VertexElement>& GetVertexElements() const
		{ return mElements; }

	Bool HasNormals() { return mHasNormals; }

private:
	void Convert(const VertexBuffer* pSrc, Float* pDst);

	TArray<DisplayList> mDisplayLists;
	TArray<VertexElement> mElements;

	void* mpData;
	UChar mVertexSize;
	Bool mHasNormals;
};

}

#endif
