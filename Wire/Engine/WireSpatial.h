// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESPATIAL_H
#define WIRESPATIAL_H

#include "WireLight.h"
#include "WireSceneObject.h"
#include "WireState.h"
#include "WireTHashTable.h"
#include "WireTransformation.h"

namespace Wire
{

class BoundingVolume;
class Culler;
class Node;
class Renderer;

class Spatial : public SceneObject
{
	WIRE_DECLARE_RTTI;

public:
	// abstract base class
	virtual ~Spatial();

	// Local and world transforms. In some situations you might need to set
	// the world transform directly and bypass the Spatial::Update()
	// mechanism. If World is set directly, the WorldIsCurrent flag should
	// be set to true. For example, inverse kinematic controllers and skin
	// controllers need this capability.
	Transformation Local;
	Transformation World;
	Bool WorldIsCurrent;

	// World bound access. In some situations you might want to set the
	// world bound directly and bypass the Spatial::UpdateGS() mechanism. If
	// WorldBound is set directly, the WorldBoundIsCurrent flag should be set
	// to true.
	Pointer<BoundingVolume> WorldBound;
	Bool WorldBoundIsCurrent;

	// Culling parameters.
	enum CullingMode
	{
		// Determine visibility state by comparing the world bounding volume
		// to culling planes.
		CULL_DYNAMIC,

		// Force the object to be culled. If a Node is culled, its entire
		// subtree is culled.
		CULL_ALWAYS,

		// Never cull the object. If a Node is never culled, its entire
		// subtree is never culled. To accomplish this, the first time such
		// a Node is encountered, the 'noCull' parameter is set to 'true' in
		// the recursive chain GetVisibleSet/OnGetVisibleSet.
		CULL_NEVER,

		MAX_CULLING_MODE
	};

	CullingMode Culling;

	// Update of geometric state and controllers. The UpdateGS() function
	// computes world transformations on the downward pass and world bounding
	// volumes on the upward pass. The UpdateBS() function just computes the
	// world bounding volumes on an upward pass. This is useful if model
	// data changes, causing the model and world bounds to change, but no
	// transformations need recomputing.
	void UpdateGS(Double appTime = -MathD::MAX_REAL, Bool isInitiator = true);
	void UpdateBS();

	// update render state
	virtual void UpdateRS(TArray<State*>* pStateStacks = NULL, TArray<Light*>*
		pLightStack = NULL, THashTable<UInt, UInt>* pStateKeys = NULL);

	// parent access (Node calls this during attach/detach of children)
	inline void SetParent(Node* pParent);
	inline Node* GetParent();

	// culling
	void OnGetVisibleSet(Culler& rCuller, Bool noCull);
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull) = 0;

	// render state
	inline UInt GetStateQuantity() const;
	inline State* GetState(UInt i) const;
	State* GetState(State::StateType type) const;
	void AttachState(State* pState);
	void DetachState(State::StateType type);
	inline void DetachAllStates();

    // light state
    inline UInt GetLightQuantity() const;
    inline Light* GetLight(UInt i = 0) const;
    void AttachLight(Light* pLight);
    inline void DetachLight(Light* pLight);
    inline void DetachAllLights();

	// geometric updates
	virtual void UpdateWorldBound() = 0;

	// Bind/Unbind all renderer related resources
	virtual void Bind(Renderer* pRenderer) = 0;
	virtual void Unbind(Renderer* pRenderer) = 0;

	// Traverse the child objects and call their MakeStatic()
	virtual void MakeStatic(Bool forceStatic = false,
		Bool duplicateShared = true) = 0;

protected:
	Spatial();

	// geometric updates
	virtual void UpdateWorldData(Double appTime);
	void PropagateBoundToRoot();

	// render state updates
	void PropagateStateFromRoot(TArray<State*>* pStateStacks,
		TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys);
	void PushState(TArray<State*>* pStateStacks, TArray<Light*>* pLightStack);
	void PopState(TArray<State*>* pStateStacks, TArray<Light*>* pLightStack);
	virtual void UpdateState(TArray<State*>* pStateStacks,
		TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys) = 0;

protected:
	// support for hierarchical scene graph
	Node* mpParent;

	// render state
	TArray<StatePtr> mStates;

	// light state
	TArray<Pointer<Light> > mLights;
};

typedef Pointer<Spatial> SpatialPtr;
#include "WireSpatial.inl"

}

#endif
