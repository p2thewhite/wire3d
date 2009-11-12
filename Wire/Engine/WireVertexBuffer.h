#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireBindable.h"
#include "WireObject.h"
#include "WireVertexAttributes.h"
#include "../Foundation/WireColorRGB.h"
#include "../Foundation/WireVector2.h"
#include "../Foundation/WireVector3.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ VertexBuffer : public Object, public Bindable
{

public:
	VertexBuffer(const VertexAttributes& rAttributes, UInt vertexQuantity);
	virtual ~VertexBuffer();

	UInt GetVertexQuantity() const { return mVertexQuantity; }

	Vector3F& Position3(UInt i);
	Vector3F Position3(UInt i) const;

	ColorRGB& Color3(UInt i, UInt unit = 0);
	ColorRGB Color3(UInt i, UInt unit = 0) const;

	Vector3F& Normal3(UInt i);
	Vector3F Normal3(UInt i) const;

	Vector2F& TCoord2(UInt i, UInt unit = 0);
	Vector2F TCoord2(UInt i, UInt unit = 0) const;

	Float* GetData();
	const Float* GetData() const;

	Float* GetPosition(UInt i = 0);
	const Float* GetPosition(UInt i = 0) const;

 	Float* GetColor(UInt i = 0, UInt unit = 0);
 	const Float* GetColor(UInt i = 0, UInt unit = 0) const;

	Float* GetNormal(UInt i = 0);
	const Float* GetNormal(UInt i = 0) const;

	Float* GetTCoord(UInt i = 0, UInt unit = 0);
	const Float* GetTCoord(UInt i = 0, UInt unit = 0) const;

	UInt GetPositionStride() const;

	const VertexAttributes& GetAttributes() const;

private:
	VertexAttributes mAttributes;

	// The number of vertices in the buffer.
	UInt mVertexQuantity;

	// The vertex buffer data.
	Float* mpChannel;
};

typedef Pointer<VertexBuffer> VertexBufferPtr;

#include "WireVertexBuffer.inl"

}

#endif
