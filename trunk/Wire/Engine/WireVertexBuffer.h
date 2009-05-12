#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ VertexBuffer : public Object
{

public:
	VertexBuffer(UInt vertexQuantity);
	virtual ~VertexBuffer();

	Vector3f& Position3(Int i);
	Vector3f Position3(Int i) const;

private:
	// The size of a single vertex in the buffer.
	UInt mVertexSize;

	// The number of vertices in the buffer.
	UInt mVertexQuantity;

	// The vertex buffer data.
	Float* mpChannel;
};

}

#endif /* WIREVERTEXBUFFER_H */
