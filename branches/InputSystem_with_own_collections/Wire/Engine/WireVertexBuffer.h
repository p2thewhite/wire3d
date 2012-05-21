// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREVERTEXBUFFER_H
#define WIREVERTEXBUFFER_H

#include "WireBuffer.h"
#include "WireColor32.h"
#include "WireVector2.h"
#include "WireVector3.h"
#include "WireVertexAttributes.h"

namespace Wire
{

class VertexBuffer : public Buffer
{
	WIRE_DECLARE_RTTI;

public:
	VertexBuffer(const VertexAttributes& rAttributes, UInt vertexQuantity,
		UsageType usage = UT_STATIC);
	VertexBuffer(const VertexBuffer* pVertexBuffer);
	virtual ~VertexBuffer();

	inline UInt GetQuantity() const;
	inline Float* GetData();
	inline const Float* GetData() const;
	inline const VertexAttributes& GetAttributes() const;

	Vector3F& Position3(UInt i);
	Vector3F Position3(UInt i) const;

	Color32& Color3(UInt i, UInt unit = 0);
	Color32 Color3(UInt i, UInt unit = 0) const;

	Color32& Color4(UInt i, UInt unit = 0);
	Color32 Color4(UInt i, UInt unit = 0) const;

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
