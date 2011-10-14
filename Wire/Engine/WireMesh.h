// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREMESH_H
#define WIREMESH_H

#include "WireObject.h"

namespace Wire
{

class BoundingVolume;
class IndexBuffer;
class VertexBuffer;

class Mesh : public Object
{
	WIRE_DECLARE_RTTI;

public:
	Mesh(VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer);

	inline VertexBuffer* GetVertexBuffer();
	inline const VertexBuffer* GetVertexBuffer() const;

	inline IndexBuffer* GetIndexBuffer();
	inline const IndexBuffer* GetIndexBuffer() const;

	inline BoundingVolume* GetModelBound();
	inline const BoundingVolume* GetModelBound() const;

	void UpdateModelBound();
	void GenerateNormals(Bool ignoreHardEdges = false);

private:
	Pointer<VertexBuffer> mspVertexBuffer;
	Pointer<IndexBuffer> mspIndexBuffer;
	Pointer<BoundingVolume> mspModelBound;
};

typedef Pointer<Mesh> MeshPtr;
#include "WireMesh.inl"

}

#endif
