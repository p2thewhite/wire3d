// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGEOMETRY_H
#define WIREGEOMETRY_H

#include "WireSpatial.h"
#include "WireRenderObject.h"

namespace Wire
{

class Geometry : public Spatial
{
	WIRE_DECLARE_RTTI;

public:
	Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer, Material*
		pMaterial = NULL);
	Geometry(Mesh* pMesh, Material* pMaterial = NULL);
	virtual ~Geometry();

	inline RenderObject* GetRenderObject();
	inline const RenderObject* GetRenderObject() const;

	inline Mesh* GetMesh();
	inline const Mesh* GetMesh() const;
	inline void SetMesh(Mesh* pMesh);

	inline Material* GetMaterial();
	inline const Material* GetMaterial() const;
	inline void SetMaterial(Material* pMaterial);

	inline TArray<Pointer<Light> >& GetLights();
	inline const TArray<Pointer<Light> >& GetLights() const;

	inline StatePtr* GetStates();
	inline const StatePtr* GetStates() const;

	inline UInt GetStateSetID() const;

	// geometric update
	virtual void UpdateWorldBound();

	// Bind/Unbind all renderer related of the RenderObject
	virtual void Bind(Renderer* pRenderer);
	virtual void Unbind(Renderer* pRenderer);

	// If World(Bound)IsCurrent or forceStatic is true, apply World transform
	// to the vertices of the mesh and set World(Bound) to identity.
	// If duplicateShared is true, shared Meshes will be duplicated before
	// being processed. Otherwise shared Meshes will not be processed.
	virtual void MakeStatic(Bool forceStatic = false,
		Bool duplicateShared = true);

protected:
	Geometry();

	// geometric update
	virtual void UpdateWorldData(Double appTime);

	// render state updates
	virtual void UpdateState(TArray<State*>* pStateStacks,
		TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

	Pointer<RenderObject> mspRenderObject;

private:
	UInt GetStateSetKey();
	void Init(Mesh* pMesh, Material* pMaterial);

	Bool VerifyKey(UInt key, UInt offset); 	// TODO: remove
};

typedef Pointer<Geometry> GeometryPtr;
#include "WireGeometry.inl"

}

#endif
