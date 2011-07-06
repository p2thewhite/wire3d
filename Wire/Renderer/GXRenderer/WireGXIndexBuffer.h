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

#include "WireTArray.h"
#include "WireTypes.h"

namespace Wire
{

class Renderer;
class IndexBuffer;
class PdrVertexBuffer;

class PdrIndexBuffer
{
public:
	PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer* pIndexBuffer);
	~PdrIndexBuffer();

	void Enable(Renderer* pRenderer);
	void Disable(Renderer* pRenderer);

	TArray<PdrVertexBuffer*>& GetPdrVBuffers() { return mPdrVBuffers; }

private:
	TArray<PdrVertexBuffer*> mPdrVBuffers;

// 	struct DisplayListEntry 
// 	{
// 			DisplayListEntry(UShort id) : mId(id)  {}
// 		private:
// 			friend class PdrIndexBuffer;
// 			UShort mId;
// 	};

};

}

#endif
