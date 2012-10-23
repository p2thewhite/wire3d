// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGXDISPLAYLIST_H
#define WIREGXDISPLAYLIST_H

#include "WireGXIndexBuffer.h"
#include "WireGXVertexFormat.h"
#include "WireTArray.h"

namespace Wire
{

class PdrRendererData;

class PdrDisplayList : public Object
{
public:
	PdrDisplayList(PdrRendererData* pRendererData, const PdrIndexBuffer&
		rIBuffer, UInt indexCount,
		const TArray<PdrVertexFormat::VertexElement>& rElements);
	~PdrDisplayList();

	inline void Draw();

private:
	void* mpData;
	UInt mSize;
};

typedef Pointer<PdrDisplayList> PdrDisplayListPtr;

#include "WireGXDisplayList.inl"

}

#endif
