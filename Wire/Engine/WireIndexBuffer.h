// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREINDEXBUFFER_H
#define WIREINDEXBUFFER_H

#include "WireBuffer.h"

namespace Wire
{

class IndexBuffer : public Buffer
{
	WIRE_DECLARE_RTTI;

public:
	IndexBuffer(UInt quantity, UsageType usage = UT_STATIC);
	virtual ~IndexBuffer();

	// Access to indices.
	inline UInt operator[] (UInt i) const;
	inline UInt& operator[] (UInt i);

	// Direct access to the index buffer data. The quantity is the number of
	// UInt elements. The number of bytes for the entire index buffer is
	// GetQuantity()*sizeof(UInt).
	inline UInt GetQuantity() const;
	inline UInt* GetData();
	inline const UInt* GetData() const;

private:
	UInt mQuantity;
	UInt* mpIndices;
};

typedef Pointer<IndexBuffer> IndexBufferPtr;

#include "WireIndexBuffer.inl"

}

#endif
