#pragma once
#ifndef WIRESPATIAL_H
#define WIRESPATIAL_H

#include "WireBoundingVolume.h"
#include "WireEffect.h"
#include "WireLight.h"
#include "WireSceneObject.h"
#include "WireState.h"
#include "WireTransformation.h"

namespace Wire
{

class Culler;

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
	BoundingVolumePtr WorldBound;
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
	virtual void UpdateRS(TArray<State*>* pGStack = NULL,
		TArray<Light*>* pLStack = NULL);

	// parent access (Node calls this during attach/detach of children)
	inline void SetParent(Spatial* pParent);
	inline Spatial* GetParent();

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

	// effect state
	inline UInt GetEffectQuantity() const;
	inline Effect* GetEffect(UInt i = 0) const;
	void AttachEffect(Effect* pEffect);
	inline void DetachEffect(Effect* pEffect);
	inline void DetachAllEffects();

protected:
	Spatial();

	// geometric updates
	virtual void UpdateWorldData(Double appTime);
	virtual void UpdateWorldBound() = 0;
	void PropagateBoundToRoot();

	// render state updates
	void PropagateStateFromRoot(TArray<State*>* pGStack,
		TArray<Light*>* pLStack);
	void PushState(TArray<State*>* pGStack, TArray<Light*>* pLStack);
	void PopState(TArray<State*>* pGStack, TArray<Light*>* pLStack);
	virtual void UpdateState(TArray<State*>* pGStack,
		TArray<Light*>* pLStack) = 0;

protected:
	// support for hierarchical scene graph
	Spatial* mpParent;

	// render state
	TArray<StatePtr> mStates;

	// light state
	TArray<LightPtr> mLights;

	// Effect state. If the effect is attached to a Geometry object, it
	// applies to that object alone. If the effect is attached to a Node
	// object, it applies to all Geometry objects in the subtree rooted at
	// the Node.
	TArray<EffectPtr> mEffects;
};

typedef Pointer<Spatial> SpatialPtr;
#include "WireSpatial.inl"
// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.


}

#endif
