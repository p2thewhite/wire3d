// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireIndexBuffer.h"

#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, IndexBuffer, Buffer);

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(UInt quantity, UsageType usage)
	:
	Buffer(usage),
	mQuantity(quantity)
{
	mpIndices = WIRE_NEW UInt[mQuantity];
}

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(UInt* pIndices, UInt quantity, UsageType usage)
	:
	Buffer(usage),
	mQuantity(quantity),
	mpIndices(pIndices)
{
}

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(const IndexBuffer* pIndexBuffer)
	:
	Buffer(pIndexBuffer->GetUsage()),
	mQuantity(pIndexBuffer->mQuantity)
{
	mpIndices = WIRE_NEW UInt[mQuantity];
	size_t size = mQuantity*sizeof(UInt);
	System::Memcpy(mpIndices, size, pIndexBuffer->mpIndices, size);
}

//----------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	// Inform all renderers using this index buffer that it is being
	// destroyed. This allows the renderer to free up any associated
	// resources.
	Renderer::UnbindAll(this);

	WIRE_DELETE[] mpIndices;
}
