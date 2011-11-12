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

#include "WireBuffer.h"
#include "WireTArray.h"

namespace Wire
{

class PdrDisplayList;
class Renderer;
class VertexAttributes;
class VertexBuffer;

class PdrVertexBuffer
{
public:
	struct VertexElement
	{
		UInt Offset;
		UChar Attr;
		UChar CompCnt;
		UChar CompType;
	};

	PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);

	inline void* Lock(Buffer::LockingMode mode);
	inline void Unlock();

	void Update(const VertexBuffer* pVertexBuffer);

	inline void SetBuffer(Renderer* pRenderer);
	inline void SetDeclaration(Renderer* pRenderer);

	inline const TArray<VertexElement>& GetDeclaration() const;

private:
	void CreateDeclaration(Renderer* pRenderer, const VertexAttributes&
		rAttributes);
	void Convert(const VertexBuffer* pSrc, Float* pDst);

	TArray<VertexElement> mDeclaration;
	void* mpData;
	UInt mVBOSize;
	UChar mVertexSize;
};

#include "WireGXVertexBuffer.inl"

}

#endif
