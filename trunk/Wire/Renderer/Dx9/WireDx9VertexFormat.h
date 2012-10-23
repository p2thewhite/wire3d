// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREDX9VERTEXFORMAT_H
#define WIREDX9VERTEXFORMAT_H

#include "WireDx9RendererData.h"
#include "WireRenderer.h"
#include "WireVertexAttributes.h"

namespace Wire
{

class Renderer;

class PdrVertexFormat
{
public:
	PdrVertexFormat(Renderer* pRenderer, const VertexAttributes&
		rVertexAttributes);
	~PdrVertexFormat();

	inline void Enable(Renderer* pRenderer);
	inline void Disable(Renderer* pRenderer);

private:
	IDirect3DVertexDeclaration9* mpDeclaration;
};

#include "WireDx9VertexFormat.inl"

}

#endif
