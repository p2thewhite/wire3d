#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireColorRGB.h"
#include "WireColorRGBA.h"
#include "WireObject.h"
#include "WireVertexAttributes.h"
#include "WireVector2.h"
#include "WireVector3.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ VertexBuffer : public Object
{
	WIRE_DECLARE_RTTI;

public:
	VertexBuffer(const VertexAttributes& rAttributes, UInt vertexQuantity);
	virtual ~VertexBuffer();

	inline UInt GetVertexQuantity() const;
	inline Float* GetData();
	inline const Float* GetData() const;
	inline const VertexAttributes& GetAttributes() const;

	Vector3F& Position3(UInt i);
	Vector3F Position3(UInt i) const;

	ColorRGB& Color3(UInt i, UInt unit = 0);
	ColorRGB Color3(UInt i, UInt unit = 0) const;

	ColorRGBA& Color4(UInt i, UInt unit = 0);
	ColorRGBA Color4(UInt i, UInt unit = 0) const;

	Vector3F& Normal3(UInt i);
	Vector3F Normal3(UInt i) const;

	Vector2F& TCoord2(UInt i, UInt unit = 0);
	Vector2F TCoord2(UInt i, UInt unit = 0) const;

	inline Float* GetPosition(UInt i = 0);
	inline const Float* GetPosition(UInt i = 0) const;

 	inline Float* GetColor(UInt i = 0, UInt unit = 0);
 	inline const Float* GetColor(UInt i = 0, UInt unit = 0) const;

	inline Float* GetNormal(UInt i = 0);
	inline const Float* GetNormal(UInt i = 0) const;

	inline Float* GetTCoord(UInt i = 0, UInt unit = 0);
	inline const Float* GetTCoord(UInt i = 0, UInt unit = 0) const;

	void GeneratePlatonicNormals();

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