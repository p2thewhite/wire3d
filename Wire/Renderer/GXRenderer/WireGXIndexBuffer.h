// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGXINDEXBUFFER_H
#define WIREGXINDEXBUFFER_H

#include "WireBuffer.h"
#include "WireTMap.h"
#include "WireTypes.h"

namespace Wire
{

class Renderer;
class IndexBuffer;
class PdrDisplayList;

class PdrIndexBuffer
{
public:
	PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer* pIndexBuffer);
	PdrIndexBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType usage);
	~PdrIndexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);

	void Update(const IndexBuffer* pIndexBuffer);

	inline UInt GetBufferSize() const;

	inline UShort* GetBuffer();
	inline const UShort* GetBuffer() const;

	inline TMap<UInt, PdrDisplayList*>& GetDisplayLists();

private:
	void CreateBuffer(Renderer* pRenderer, UInt size, Buffer::UsageType
		usage);
	void Copy(const IndexBuffer* pIndexBuffer, void* pBuffer, UInt offset);

	TMap<UInt, PdrDisplayList*> mDisplayLists;

	UShort* mpBuffer;
	UInt mBufferSize;
};

#include "WireGXIndexBuffer.inl"

}

#endif
