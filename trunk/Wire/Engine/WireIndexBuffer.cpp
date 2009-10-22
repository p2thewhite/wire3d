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
	// Inform all renderers using this index buffer that it is being
	// destroyed. This allows the renderer to free up any associated
	// resources.
	Release();

	WIRE_DELETE[] mpIndices;
}
