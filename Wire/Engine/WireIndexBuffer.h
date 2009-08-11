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

private:
	UInt mQuantity;
	Int* mpIndices;
};

typedef Pointer<IndexBuffer> IndexBufferPtr;

}

#endif
