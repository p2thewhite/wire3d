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
	~PdrIndexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);

private:
	friend class Renderer;
	struct DisplayListEntry 
	{
		DisplayListEntry() {}
		DisplayListEntry(PdrDisplayList* pDisplayList)
			:
			DisplayList(pDisplayList), References(1) {}

			PdrDisplayList* DisplayList;
			UInt References;
	};

	TMap<UShort, DisplayListEntry> mDisplayLists;
};

}

#endif
