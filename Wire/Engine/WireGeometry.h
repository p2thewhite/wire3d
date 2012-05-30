// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGEOMETRY_H
#define WIREGEOMETRY_H

#include "WireSpatial.h"
#include "WireMaterial.h"

namespace Wire
{

class IndexBuffer;
class Light;
class Mesh;
class VertexBuffer;

class Geometry : public Spatial
{
	WIRE_DECLARE_RTTI;

public:
	Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer, Material*
		pMaterial = NULL);
	Geometry(Mesh* pMesh, Material* pMaterial = NULL);
	virtual ~Geometry();

	// geometric updates
	virtual void UpdateWorldBound();

	inline Mesh* GetMesh();
	inline const Mesh* GetMesh() const;
	inline void SetMesh(Mesh* pMesh);

	inline Material* GetMaterial();
	inline const Material* GetMaterial() const;
	inline void SetMaterial(Material* pMaterial);

	// Duplicate the mesh (only if shared), apply World transform to vertices,
	// set World(Bound) to identity and World(Bound)IsCurrent to true.
	void MakeStatic();

	// member access
	StatePtr States[State::MAX_STATE_TYPE];
	TArray<Pointer<Light> > Lights;

	// Control over which continuous part of the mesh is rendered.
	// Default values render the entire mesh.
	UInt ActiveIndexCount;	// default: quantity of the mesh's indexbuffer
	UInt StartIndex;		// default: 0

	// Identical IDs among different Geometry objects mean that all their
	// States[] and Lights are identical. This is used for sorting Geometry
	// by render state.
	UInt StateSetID;

protected:
	Geometry();
	void Init();

	// render state updates
	virtual void UpdateState(TArray<State*>* pStateStacks,
		TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

private:
	UInt GetStateSetKey();

	Pointer<Mesh> mspMesh;
	Pointer<Material> mspMaterial;

	// TODO: remove
	Bool VerifyKey(UInt key, UInt offset);
};

typedef Pointer<Geometry> GeometryPtr;
#include "WireGeometry.inl"

}

#endif
