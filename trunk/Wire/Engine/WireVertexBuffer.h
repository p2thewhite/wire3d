#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireObject.h"
#include "WireVertexAttributes.h"
#include "../Foundation/WireVector2.h"
#include "../Foundation/WireVector3.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ VertexBuffer : public Object
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

	Vector3f& Normal3(UInt i);
	Vector3f Normal3(UInt i) const;

	Vector2f& TCoord2(UInt i);
	Vector2f TCoord2(UInt i) const;

private:
	VertexAttributes mAttributes;

	// The number of vertices in the buffer.
	UInt mVertexQuantity;

	// The vertex buffer data.
	Float* mpChannel;
};

typedef Pointer<VertexBuffer> VertexBufferPtr;

}

#endif /* WIREVERTEXBUFFER_H */
