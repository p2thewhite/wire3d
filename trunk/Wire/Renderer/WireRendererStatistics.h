// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRERENDERERSTATISTICS_H
#define WIRERENDERERSTATISTICS_H

#include "WireTypes.h"

namespace Wire
{

class Text;

class RendererStatistics
{
public:
	// Drawing and Renderer bound memory statistics

	// accumulated number of draw calls and triangles drawn since last Reset()
	inline UInt GetDrawCalls() const;
	inline UInt GetBatchedStatic() const;
	inline UInt GetBatchedDynamic() const;
	inline UInt GetTriangles() const;

	// number of buffers and their total size bound to the Renderer
	inline UInt GetVBOCount() const;
	inline UInt GetVBOTotalSize() const;
	inline UInt GetIBOCount() const;
	inline UInt GetIBOTotalSize() const;
	inline UInt GetTextureCount() const;
	inline UInt GetTextureTotalSize() const;

	void Reset();
//	void ToText(Text* pText);

private:
	// accumulated number of draw calls and triangles drawn since last
	// Reset()
	UInt mDrawCalls;
	UInt mBatchedStatic;
	UInt mBatchedDynamic;
	UInt mTriangles;

	// number of buffers and their total size bound to the Renderer
	UInt mVBOCount;
	UInt mVBOTotalSize;
	UInt mIBOCount;
	UInt mIBOTotalSize;
	UInt mTextureCount;
	UInt mTextureTotalSize;

	friend class Renderer;
};

#include "WireRendererStatistics.inl"

}

#endif
