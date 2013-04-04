// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREMESH_H
#define WIREMESH_H

#include "WireTArray.h"
#include "WireVertexBuffer.h"

namespace Wire
{

class BoundingVolume;
class IndexBuffer;

class Mesh : public Object
{
	WIRE_DECLARE_RTTI;

public:
	Mesh(VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer);
	Mesh(VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer,
		UInt startIndex, UInt indexCount);
	Mesh(TArray<VertexBufferPtr>& rVertexBuffers, IndexBuffer* pIndexBuffer);
	Mesh(TArray<VertexBufferPtr>& rVertexBuffers, IndexBuffer* pIndexBuffer,
		UInt startIndex, UInt indexCount);
	// shallow copy (shared Index- and Vertexbuffers)
	Mesh(const Mesh* pMesh);
	Mesh(const Mesh* pMesh, UInt startIndex, UInt indexCount);
	virtual ~Mesh();

	inline VertexBuffer* GetVertexBuffer(UInt streamIndex = 0);
	inline const VertexBuffer* GetVertexBuffer(UInt streamIndex = 0) const;
	inline const TArray<VertexBufferPtr>& GetVertexBuffers() const;
	void SetVertexBuffer(VertexBuffer* pVertexBuffer);
	void SetVertexBuffers(const TArray<VertexBufferPtr>& rVertexBuffers);
	VertexBuffer* GetPositionBuffer();
	const VertexBuffer* GetPositionBuffer() const;
	VertexBuffer* GetNormalBuffer();
	const VertexBuffer* GetNormalBuffer() const;

	inline IndexBuffer* GetIndexBuffer();
	inline const IndexBuffer* GetIndexBuffer() const;

	inline BoundingVolume* GetModelBound();
	inline const BoundingVolume* GetModelBound() const;

	inline UInt GetVertexQuantity() const;

	// Control over which continuous part of the mesh is rendered.
	// Default values render the entire mesh.
	inline UInt GetStartIndex() const;
	void SetStartIndex(UInt startIndex, Bool updateModelBound = false);
	inline UInt GetIndexCount() const;
	void SetIndexCount(UInt indexCount, Bool updateModelBound = false);
	inline Bool IsDirty() const;

	void UpdateModelBound();
	void GenerateNormals(Bool ignoreHardEdges = false);

	Bool HasNormal() const;
	Bool HasColor() const;

private:
	void Init(IndexBuffer* pIndexBuffer, UInt startIndex, UInt indexCount);

	TArray<Pointer<VertexBuffer> > mVertexBuffers;
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
