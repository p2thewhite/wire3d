// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
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
	mpIndices = WIRE_NEW UShort[mQuantity];
}

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(UShort* pIndices, UInt quantity, UsageType usage)
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
	mpIndices = WIRE_NEW UShort[mQuantity];
	size_t size = mQuantity*sizeof(UShort);
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

//----------------------------------------------------------------------------
void IndexBuffer::Copy(UShort* pDst, UShort offset)
{
	if (offset == 0)
	{
		if (pDst != GetData())
		{
			const UInt size = GetQuantity() * sizeof(UShort);
			System::Memcpy(pDst, size, GetData(), size);
		}

		return;
	}

	UShort* pSrc = GetData();
	if ((GetQuantity() % 3) == 0)
	{
		for (UInt i = 0; i < GetQuantity(); i+=3)
		{
			pDst[i] = pSrc[i] + offset;
			pDst[i+1] = pSrc[i+1] + offset;
			pDst[i+2] = pSrc[i+2] + offset;
		}
	}
	else
	{
		for (UInt i = 0; i < GetQuantity(); i++)
		{
			pDst[i] = pSrc[i] + offset;
		}
	}
}
