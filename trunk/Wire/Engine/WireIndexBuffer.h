#pragma once
#ifndef WIREINDEXBUFFER_H
#define WIREINDEXBUFFER_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ IndexBuffer : public Object
{

public:
	IndexBuffer(UInt quantity);
	virtual ~IndexBuffer();

	// Access to indices.
	UInt operator[] (UInt i) const;
	UInt& operator[] (UInt i);

	UInt GetIndexQuantity() const;

private:
	UInt mQuantity;
	UInt* mpIndices;
};

typedef Pointer<IndexBuffer> IndexBufferPtr;

#include "WireIndexBuffer.inl"

}

#endif
