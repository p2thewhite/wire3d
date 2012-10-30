// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGXDisplayList.h"

#include "WireGXRendererData.h"
#include "WireIndexBuffer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrDisplayList::PdrDisplayList(PdrRendererData* pRendererData,
	const PdrIndexBuffer& rIBuffer, UInt indexCount, 
	const TArray<PdrVertexFormat::VertexElement>& rElements)
{
	// Note that the display-list buffer area must be forced out of
	// the CPU cache since it will be written using the write-gather pipe
	const UInt maxSize = ((8 + indexCount * rElements.
		GetQuantity()*2) & 0xFFFFFFE0) + 64 ;
	mpData = memalign(32, maxSize);
	WIRE_ASSERT(mpData);
	DCInvalidateRange(mpData, maxSize);

	GXBeginDisplayList(mpData, maxSize);
	pRendererData->Draw(rElements, rIBuffer.GetBuffer(), indexCount, 0);
	mSize = GXEndDisplayList();
	WIRE_ASSERT(mSize);

	DCFlushRange(mpData, maxSize);
}

//----------------------------------------------------------------------------
PdrDisplayList::~PdrDisplayList()
{
	free(mpData);	// allocated using memalign
}