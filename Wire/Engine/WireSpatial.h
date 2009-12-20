#pragma once
#ifndef WIRESPATIAL_H
#define WIRESPATIAL_H

#include "WireBoundingVolume.h"
#include "WireGlobalState.h"
#include "WireSceneObject.h"
#include "WireTransformation.h"

namespace Wire
{

class Culler;

class /*WIRE_ENGINE_ITEM*/ Spatial : public SceneObject
{
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
	bool WorldIsCurrent;

	// World bound access. In some situations you might want to set the
	// world bound directly and bypass the Spatial::UpdateGS() mechanism. If
	// WorldBound is set directly, the WorldBoundIsCurrent flag should be set
	// to true.
	BoundingVolumePtr WorldBound;
	bool WorldBoundIsCurrent;

	// Culling parameters.
	enum CullingMode
	{
		// Determine visibility state by comparing the world bounding volume
		// to culling planes.
		CULL_DYNAMIC,

		// Force the object to be culled.  If a Node is culled, its entire
		// subtree is culled.
		CULL_ALWAYS,

		// Never cull the object.  If a Node is never culled, its entire
		// subtree is never culled.  To accomplish this, the first time such
		// a Node is encountered, the bNoCull parameter is set to 'true' in
		// the recursive chain GetVisibleSet/OnGetVisibleSet.
		CULL_NEVER,

		MAX_CULLING_MODE
	};

	CullingMode Culling;

	// Update of geometric state and controllers. The UpdateGS function
	// computes world transformations on the downward pass and world bounding
	// volumes on the upward pass. The UpdateBS function just computes the
	// world bounding volumes on an upward pass. This is useful if model
	// data changes, causing the model and world bounds to change, but no
	// transformations need recomputing.
	void UpdateGS(Double appTime = -MathD::MAX_REAL, Bool isInitiator = true);
	void UpdateBS();

	// update of render state
	virtual void UpdateRS(TArray<GlobalState*>* pStack = NULL);

	// Parent access (Node calls this during attach/detach of children)
	void SetParent(Spatial* pkParent);
	Spatial* GetParent();

	// culling
	void OnGetVisibleSet(Culler& rCuller, Bool noCull);
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull) = 0;

	// global state
	UInt GetGlobalStateQuantity() const;
	GlobalState* GetGlobalState(UInt i) const;
	GlobalState* GetGlobalState(GlobalState::StateType type) const;
	void AttachGlobalState(GlobalState* pState);
	void DetachGlobalState(GlobalState::StateType type);
	void DetachAllGlobalStates();

protected:
	Spatial();

	// geometric updates
	virtual void UpdateWorldData(Double appTime);
	virtual void UpdateWorldBound() = 0;
	void PropagateBoundToRoot();

	// render state updates
	void PropagateStateFromRoot(TArray<GlobalState*>* pStack);
	void PushState(TArray<GlobalState*>* pStack);
	void PopState(TArray<GlobalState*>* pStack);
	virtual void UpdateState(TArray<GlobalState*>* pStack) = 0;

private:
	// support for hierarchical scene graph
	Spatial* mpParent;

	// global render state
	TArray<GlobalStatePtr> mGlobalStates;
};

typedef Pointer<Spatial> SpatialPtr;
#include "WireSpatial.inl"

}

#endif
