#include "WireIndexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(UInt quantity)
	:
	mQuantity(quantity)
{
	WIRE_ASSERT(mQuantity > 0);

	mpIndices = WIRE_NEW Int[mQuantity];
}

//----------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	if (mpIndices)
	{
		WIRE_DELETE[] mpIndices;
	}
}
