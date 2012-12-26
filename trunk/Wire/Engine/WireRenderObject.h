// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRERENDEROBJECT_H
#define WIRERENDEROBJECT_H

#include "WireObject.h"
#include "WireState.h"
#include "WireTransformation.h"

namespace Wire
{

class IndexBuffer;
class Light;
class Material;
class Mesh;
class VertexBuffer;

class RenderObject: public Object
{
	WIRE_DECLARE_RTTI;

public:
	RenderObject(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer, Material*
		pMaterial = NULL);
	RenderObject(Mesh* pMesh, Material* pMaterial = NULL);
	virtual ~RenderObject();

	inline Mesh* GetMesh();
	inline const Mesh* GetMesh() const;
	inline void SetMesh(Mesh* pMesh);

	inline Material* GetMaterial();
	inline const Material* GetMaterial() const;
	inline void SetMaterial(Material* pMaterial);

	inline TArray<Pointer<Light> >& GetLights();
	inline const TArray<Pointer<Light> >& GetLights() const;
	inline Light* GetLight(UInt i = 0) const;

	inline StatePtr* GetStates();
	inline const StatePtr* GetStates() const;
	inline State* GetState(State::StateType type) const;

	inline UInt GetStateSetID() const;
	inline void SetStateSetID(UInt stateSetID);

	Transformation World;

private:
	Pointer<Mesh> mspMesh;
	Pointer<Material> mspMaterial;

	StatePtr mStates[State::MAX_STATE_TYPE];
	TArray<Pointer<Light> > mLights;

	// Identical IDs of different RenderObjects mean that all their
	// States[] and Lights are identical. This is used for sorting by
	// render state.
	UInt mStateSetID;
};

typedef Pointer<RenderObject> RenderObjectPtr;
#include "WireRenderObject.inl"

}

#endif
