// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREMESH_H
#define WIREMESH_H

#include "WireObject.h"
#include "WireTArray.h"

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
	Mesh(VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer,
		UInt startIndex, UInt indexCount);

	inline VertexBuffer* GetVertexBuffer(UInt streamIndex = 0);
	inline const VertexBuffer* GetVertexBuffer(UInt streamIndex = 0) const;
	inline TArray<Pointer<VertexBuffer> >& GetVertexBuffers();
	inline const TArray<Pointer<VertexBuffer> >& GetVertexBuffers() const;

	inline IndexBuffer* GetIndexBuffer();
	inline const IndexBuffer* GetIndexBuffer() const;

	inline BoundingVolume* GetModelBound();
	inline const BoundingVolume* GetModelBound() const;

	// Control over which continuous part of the mesh is rendered.
	// Default values render the entire mesh.
	inline UInt GetStartIndex();
	void SetStartIndex(UInt startIndex, Bool updateModelBound = false);
	inline UInt GetIndexCount();
	void SetIndexCount(UInt indexCount, Bool updateModelBound = false);
	inline Bool IsDirty();

	void UpdateModelBound();
	void GenerateNormals(Bool ignoreHardEdges = false);

private:
	TArray<Pointer<VertexBuffer> > mspVertexBuffers;
	Pointer<IndexBuffer> mspIndexBuffer;
	Pointer<BoundingVolume> mspModelBound;

	UInt mStartIndex;
	UInt mIndexCount;
	Bool mIsDirty;
};

typedef Pointer<Mesh> MeshPtr;
#include "WireMesh.inl"

}

#endif
