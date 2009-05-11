#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ VertexBuffer : public Object
{

public:
	VertexBuffer(unsigned int vertexQuantity);
	virtual ~VertexBuffer();

	Vector3f& Position3(int i);
	Vector3f Position3(int i) const;

private:
	// The size of a single vertex in the buffer.
	unsigned int mVertexSize;

	// The number of vertices in the buffer.
	unsigned int mVertexQuantity;

	// The vertex buffer data.
	void* mpChannel;

	Float mChannel[24*3];
};

}

#endif /* WIREVERTEXBUFFER_H */
