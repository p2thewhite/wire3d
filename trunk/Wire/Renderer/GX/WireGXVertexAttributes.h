// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGXVERTEXATTRIBUTES_H
#define WIREGXVERTEXATTRIBUTES_H

#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include "WireVertexAttributes.h"

namespace Wire
{

class PdrVertexAttributes
{
public:
	PdrVertexAttributes(Renderer* pRenderer, const VertexAttributes&
		rVertexAttributes);

	inline void Enable(Renderer* pRenderer);

	inline UInt GetVertexSize() const;

private:
	struct VertexElement
	{
		UInt Offset;
		UChar Attr;
		UChar CompCnt;
		UChar CompType;
	};

	TArray<VertexElement> mDeclaration;
	UChar mVertexSize;
};

#include "WireGXVertexAttributes.inl"

}

#endif
