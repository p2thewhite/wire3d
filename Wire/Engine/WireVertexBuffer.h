#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireObject.h"
#include "WireVertexAttributes.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ VertexBuffer : public Object
{

public:
	VertexBuffer(const VertexAttributes& rAttributes, UInt vertexQuantity,
		Bool isInterleaved = false);
	virtual ~VertexBuffer();

	UInt GetQuantity() const { return mVertexQuantity; }

	Vector3f& Position3(UInt i);
	Vector3f Position3(UInt i) const;

	Vector3f& Color3(UInt i);
	Vector3f Color3(UInt i) const;

private:
	VertexAttributes mAttributes;

	// The number of vertices in the buffer.
	UInt mVertexQuantity;

	// The vertex buffer data.
	Float* mpChannel;
};

}

#endif /* WIREVERTEXBUFFER_H */
