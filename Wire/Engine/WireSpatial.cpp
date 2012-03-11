// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireSpatial.h"

#include "WireBoundingVolume.h"
#include "WireCuller.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Spatial, SceneObject);

//----------------------------------------------------------------------------
Spatial::Spatial()
	:
	WorldIsCurrent(false),
	WorldBound(BoundingVolume::Create()),
	WorldBoundIsCurrent(false),
	Culling(CULL_DYNAMIC),
	mpParent(NULL)
{
}

//----------------------------------------------------------------------------
Spatial::~Spatial()
{
}

//----------------------------------------------------------------------------
void Spatial::UpdateGS(Double appTime, Bool isInitiator)
{
	UpdateWorldData(appTime);
	UpdateWorldBound();
	if (isInitiator)
	{
		PropagateBoundToRoot();
	}
}

//----------------------------------------------------------------------------
void Spatial::UpdateBS()
{
	UpdateWorldBound();
	PropagateBoundToRoot();
}

//----------------------------------------------------------------------------
void Spatial::UpdateWorldData(Double appTime)
{
	// update any controllers associated with this object
	UpdateControllers(appTime);

	// update world transforms
	if (!WorldIsCurrent)
	{
		if (mpParent)
		{
			World.Product(mpParent->World, Local);
		}
		else
		{
			World = Local;
		}
	}
}

//----------------------------------------------------------------------------
void Spatial::PropagateBoundToRoot()
{
	if (mpParent)
	{
		mpParent->UpdateWorldBound();
		mpParent->PropagateBoundToRoot();
	}
}

//----------------------------------------------------------------------------
void Spatial::OnGetVisibleSet(Culler& rCuller, Bool noCull)
{
	if (Culling == CULL_ALWAYS)
	{
		return;
	}

	if (Culling == CULL_NEVER)
	{
		noCull = true;
	}

	UInt savePlaneState = rCuller.GetPlaneState();
	if (noCull || rCuller.IsVisible(WorldBound))
	{
		RenderUpdateControllers();
		GetVisibleSet(rCuller, noCull);
	}

	rCuller.SetPlaneState(savePlaneState);
}

//----------------------------------------------------------------------------
State* Spatial::GetState(State::StateType type) const
{
	// check if type of state already exists
	for (UInt i = 0; i < mStates.GetQuantity(); i++)
	{
		if (mStates[i]->GetStateType() == type)
		{
			// type of state exists, return it
			return mStates[i];
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------
void Spatial::AttachState(State* pState)
{
	WIRE_ASSERT(pState);

	// Check if this type of state is already in the list.
	for (UInt i = 0; i < mStates.GetQuantity(); i++)
	{
		if (mStates[i]->GetStateType() == pState->GetStateType())
		{
			// This type of state already exists, so replace it.
			mStates[i] = pState;
			return;
		}
	}

	// This type of state is not in the current list, so add it.
	mStates.Append(pState);
}

//----------------------------------------------------------------------------
void Spatial::DetachState(State::StateType type)
{
	for (UInt i = 0; i < mStates.GetQuantity(); i++)
	{
		State* pState = mStates[i];

		if (pState->GetStateType() == type)
		{
			mStates.RemoveAt(i);
			return;
		}
	}
}

//----------------------------------------------------------------------------
void Spatial::AttachLight(Light* pLight)
{
	WIRE_ASSERT(pLight);

	// Check if this light is already in the list.
	for (UInt i = 0; i < mLights.GetQuantity(); i++)
	{
		if (mLights[i] == pLight)
		{
			// This Light already exists.
			return;
		}
	}

	// This light is not in the current list, so add it.
	mLights.Append(pLight);
}

//----------------------------------------------------------------------------
void Spatial::AttachEffect(Effect* pEffect)
{
	WIRE_ASSERT(pEffect);

	// Check if the effect is already in the list.
	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
	{
		if (mEffects[i] == pEffect)
		{
			// The effect already exists, so do nothing.
			return;
		}
	}

	// The effect is not in the current list, so add it.
	mEffects.Append(pEffect);
}

//----------------------------------------------------------------------------
void Spatial::UpdateRS(TArray<State*>* pStateStacks, TArray<Light*>*
	pLightStack, THashTable<UInt, UInt>* pStateKeys)
{
	Bool isInitiator = (pStateStacks == NULL);

	if (isInitiator)
	{
		const UInt growBy = 16;

		// The order of preference is
		//   (1) Default global states are used.
		//   (2) Geometry can override them, but if global state FOOBAR
		//       has not been pushed to the Geometry leaf node, then
		//       the current FOOBAR remains in effect (rather than the
		//       default FOOBAR being used).
		//   (3) Effect can override default or Geometry render states.
		pStateStacks = WIRE_NEW TArray<State*>[State::MAX_STATE_TYPE];

		for (UInt i = 0; i < State::MAX_STATE_TYPE; i++)
		{
			pStateStacks[i].SetGrowBy(growBy);
			pStateStacks[i].Append(State::Default[i]);
		}

        // stack has no lights initially
        pLightStack = WIRE_NEW TArray<Light*>(0, growBy);

		// Geometry identifies sets of identical states and shares their ID
		pStateKeys = WIRE_NEW THashTable<UInt, UInt>;

		// traverse to root and push states from root to this node
		PropagateStateFromRoot(pStateStacks, pLightStack, pStateKeys);
	}
	else
	{
		// push states at this node
		PushState(pStateStacks, pLightStack);
	}

	// propagate the new state to the subtree rooted here
	UpdateState(pStateStacks, pLightStack, pStateKeys);

	if (isInitiator)
	{
		WIRE_DELETE[] pStateStacks;
		WIRE_DELETE pLightStack;
		WIRE_DELETE pStateKeys;
	}
	else
	{
		// pop states at this node
		PopState(pStateStacks, pLightStack);
	}
}

//----------------------------------------------------------------------------
void Spatial::PropagateStateFromRoot(TArray<State*>* pStateStacks,
	TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys)
{
	// traverse to root to allow downward state propagation
	if (mpParent)
	{
		mpParent->PropagateStateFromRoot(pStateStacks, pLightStack,
			pStateKeys);
	}

	// push states onto current render state stack
	PushState(pStateStacks, pLightStack);
}

//----------------------------------------------------------------------------
void Spatial::PushState(TArray<State*>* pStateStacks,
	TArray<Light*>* pLightStack)
{
	for (UInt i = 0; i < mStates.GetQuantity(); i++)
	{
		State::StateType type = mStates[i]->GetStateType();
		pStateStacks[type].Append(mStates[i]);
	}

	for (UInt i = 0; i < mLights.GetQuantity(); i++)
	{
		pLightStack->Append(mLights[i]);
	}
}

//----------------------------------------------------------------------------
void Spatial::PopState(TArray<State*>* pStateStacks,
	TArray<Light*>* pLightStack)
{
	for (UInt i = 0; i < mStates.GetQuantity(); i++)
	{
		State::StateType type = mStates[i]->GetStateType();
		pStateStacks[type].RemoveLast();
	}

	for (UInt i = 0; i < mLights.GetQuantity(); i++)
	{
		pLightStack->RemoveLast();
	}
}
