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
#include "WireGXVertexAttributes.h"

namespace Wire
{

class PdrDisplayList;
class Renderer;
class VertexBuffer;

class PdrVertexBuffer
{
public:
	PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer* pVertexBuffer);
	PdrVertexBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType usage);
	~PdrVertexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);

	inline void* Lock(Buffer::LockingMode mode);
	inline void Unlock();

	void Update(const VertexBuffer* pVertexBuffer);
	void Update(const VertexBuffer* pVertexBuffer, UInt count,
		UInt offset = 0);

	inline UInt GetBufferSize() const;

	void CreateDeclaration(Renderer* pRenderer, const VertexAttributes&
		rAttributes);

	inline const TArray<PdrVertexAttributes::VertexElement>& GetDeclaration() const;

private:
	void CreateBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType
		usage);
	inline void SetBuffer(Renderer* pRenderer, UInt vertexSize);

	void* mpBuffer;
	UInt mBufferSize;

	inline void SetDeclaration(Renderer* pRenderer);
	PdrVertexAttributes* mpPdrVertexAttributes;
};

#include "WireGXVertexBuffer.inl"

}

#endif
