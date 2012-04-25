// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
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
	PdrVertexBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType usage);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);

	inline void* Lock(Buffer::LockingMode mode);
	inline void Unlock();

	void Update(const VertexBuffer* pVertexBuffer);

	inline void SetBuffer(Renderer* pRenderer, UInt vertexSize);
	inline void SetDeclaration(Renderer* pRenderer);

	inline UInt GetBufferSize() const;

	inline const TArray<VertexElement>& GetDeclaration() const;

private:
	void CreateBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType
		usage);
	void CreateDeclaration(Renderer* pRenderer, const VertexAttributes&
		rAttributes);

	TArray<VertexElement> mDeclaration;
	void* mpBuffer;
	UInt mBufferSize;
	UChar mVertexSize;
};

#include "WireGXVertexBuffer.inl"

}

#endif
