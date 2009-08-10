#pragma once
#ifndef WIREINDEXBUFFER_H
#define WIREINDEXBUFFER_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ IndexBuffer : public Object
{

public:
	IndexBuffer(Int quantity, Int size = sizeof(Int));
	virtual ~IndexBuffer();

private:
	Int mQuantity;
	Int mSize;
	Int* mpIndices;
};

typedef Pointer<IndexBuffer> IndexBufferPtr;

}

#endif
