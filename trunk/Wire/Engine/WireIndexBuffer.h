#pragma once
#ifndef WIREINDEXBUFFER_H
#define WIREINDEXBUFFER_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ IndexBuffer : public Object
{
	WIRE_DECLARE_RTTI;

public:
	IndexBuffer(UInt quantity);
	virtual ~IndexBuffer();

	// Access to indices.
	UInt operator[] (UInt i) const;
	UInt& operator[] (UInt i);

	// Direct access to the index buffer data. The quantity is the number of
	// UInt elements. The number of bytes for the entire index buffer is
	// GetIndexQuantity()*sizeof(UInt).
	UInt GetIndexQuantity() const;
	UInt* GetData();
	const UInt* GetData() const;

private:
	UInt mQuantity;
	UInt* mpIndices;
};

typedef Pointer<IndexBuffer> IndexBufferPtr;

#include "WireIndexBuffer.inl"

}

#endif
