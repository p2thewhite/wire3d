#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireObject.h"
#include "WireVertexAttributes.h"
#include "../Foundation/WireColorRGB.h"
#include "../Foundation/WireVector2.h"
#include "../Foundation/WireVector3.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ VertexBuffer : public Object
{

public:
	VertexBuffer(const VertexAttributes& rAttributes, UInt vertexQuantity);
	virtual ~VertexBuffer();

	UInt GetVertexQuantity() const { return mVertexQuantity; }

	Vector3F& Position3(UInt i);
	Vector3F Position3(UInt i) const;

	ColorRGB& Color3(UInt i);
	ColorRGB Color3(UInt i) const;

	Vector3F& Normal3(UInt i);
	Vector3F Normal3(UInt i) const;

	Vector2F& TCoord2(UInt i);
	Vector2F TCoord2(UInt i) const;

private:
	VertexAttributes mAttributes;

	// The number of vertices in the buffer.
	UInt mVertexQuantity;

	// The vertex buffer data.
	Float* mpChannel;
};

typedef Pointer<VertexBuffer> VertexBufferPtr;

}

#endif
