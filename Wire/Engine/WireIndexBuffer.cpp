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
	WIRE_DELETE[] mpIndices;
}
