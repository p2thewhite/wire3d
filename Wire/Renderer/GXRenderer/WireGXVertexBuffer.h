// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGXVERTEXBUFFER_H
#define WIREGXVERTEXBUFFER_H

#include "WireTArray.h"
#include "WireTypes.h"

namespace Wire
{

class PdrDisplayList;
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

	PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);
	void Update(const VertexBuffer* pVertexBuffer);

	TArray<PdrDisplayList*>& GetDisplayLists() { return mDisplayLists; }
	const TArray<VertexElement>& GetVertexElements() const
		{ return mElements; }

	inline Bool HasNormals();

private:
	void Convert(const VertexBuffer* pSrc, Float* pDst);

	TArray<PdrDisplayList*> mDisplayLists;
	TArray<VertexElement> mElements;

	void* mpData;
	UShort mElementId;
	UChar mVertexSize;
};

#include "WireGXVertexBuffer.inl"

}

#endif
