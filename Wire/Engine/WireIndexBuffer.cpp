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
IndexBuffer::~IndexBuffer()
{
	// Inform all renderers using this index buffer that it is being
	// destroyed. This allows the renderer to free up any associated
	// resources.
	Renderer::UnbindAll(this);

	WIRE_DELETE[] mpIndices;
}
