// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGXDISPLAYLIST_H
#define WIREGXDISPLAYLIST_H

#include "WireIndexBuffer.h"
#include "WireGXVertexBuffer.h"
#include "WireObject.h"
#include "WireTArray.h"
#include "WireVertexBuffer.h"

namespace Wire
{

class PdrRendererData;

class PdrDisplayList : public Object
{
public:
	PdrDisplayList(PdrRendererData* pRendererData, const IndexBuffer&
		rIBuffer, const TArray<PdrVertexBuffer::VertexElement>& rElements);
	~PdrDisplayList();

	inline void Draw();

	PdrIndexBuffer* RegisteredIBuffer;

private:
	void* mpData;
	UInt mSize;
};

typedef Pointer<PdrDisplayList> PdrDisplayListPtr;

#include "WireGXDisplayList.inl"

}

#endif
