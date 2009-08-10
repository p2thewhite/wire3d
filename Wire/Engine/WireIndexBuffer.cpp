#include "WireIndexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(Int quantity, Int size)
	:
	mQuantity(quantity),
	mSize(size)
{
	WIRE_ASSERT(mQuantity > 0 && mSize > 0);

	if (sizeof(Int) == size)
	{
		mpIndices = WIRE_NEW Int[quantity];
	}
	else if (sizeof(UShort) == size)
	{
		UShort* pIndices = WIRE_NEW UShort[quantity];
		mpIndices = reinterpret_cast<Int*>(pIndices);
	}
	else if (sizeof(UChar) == size)
	{
		UChar* pIndices = WIRE_NEW UChar[quantity];
		mpIndices = reinterpret_cast<Int*>(pIndices);
	}
	else
	{
		WIRE_ASSERT(false);
		mpIndices = NULL;
	}
}

//----------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	if (mpIndices)
	{
		WIRE_DELETE[] mpIndices;
	}
}
