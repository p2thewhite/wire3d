#include "WireIndexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(UInt quantity)
	:
	mQuantity(quantity)
{
	mpIndices = WIRE_NEW UInt[mQuantity];
}

//----------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	if (mpIndices)
	{
		WIRE_DELETE[] mpIndices;
	}
}
