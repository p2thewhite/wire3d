#include "WireSpatial.h"

#include "WireCuller.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Spatial, SceneObject);

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
		GetVisibleSet(rCuller, noCull);
	}

	rCuller.SetPlaneState(savePlaneState);
}

//----------------------------------------------------------------------------
GlobalState* Spatial::GetGlobalState(GlobalState::StateType type) const
{
	// check if type of state already exists
	for (UInt i = 0; i < mGlobalStates.GetQuantity(); i++)
	{
		if (mGlobalStates[i]->GetStateType() == type)
		{
			// type of state exists, return it
			return mGlobalStates[i];
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------
void Spatial::AttachGlobalState(GlobalState* pState)
{
	WIRE_ASSERT(pState);

	// Check if this type of state is already in the list.
	for (UInt i = 0; i < mGlobalStates.GetQuantity(); i++)
	{
		if (mGlobalStates[i]->GetStateType() == pState->GetStateType())
		{
			// This type of state already exists, so replace it.
			mGlobalStates[i] = pState;
			return;
		}
	}

	// This type of state is not in the current list, so add it.
	mGlobalStates.Append(pState);
}

//----------------------------------------------------------------------------
void Spatial::DetachGlobalState(GlobalState::StateType type)
{
	for (UInt i = 0; i < mGlobalStates.GetQuantity(); i++)
	{
		GlobalState* pState = mGlobalStates[i];

		if (pState->GetStateType() == type)
		{
			mGlobalStates.RemoveAt(i);
			return;
		}
	}
}

//----------------------------------------------------------------------------
void Spatial::UpdateRS(TArray<GlobalState*>* pStack)
{
	Bool isInitiator = (pStack == NULL);

	if (isInitiator)
	{
		// The order of preference is
		//   (1) Default global states are used.
		//   (2) Geometry can override them, but if global state FOOBAR
		//       has not been pushed to the Geometry leaf node, then
		//       the current FOOBAR remains in effect (rather than the
		//       default FOOBAR being used).
		//   (3) Effect can override default or Geometry render states.
		pStack = WIRE_NEW TArray<GlobalState*>[GlobalState::MAX_STATE_TYPE];

		for (UInt i = 0; i < GlobalState::MAX_STATE_TYPE; i++)
		{
			pStack[i].Append(NULL);
		}

		// traverse to root and push states from root to this node
		PropagateStateFromRoot(pStack);
	}
	else
	{
		// push states at this node
		PushState(pStack);
	}

	// propagate the new state to the subtree rooted here
	UpdateState(pStack);

	if (isInitiator)
	{
		WIRE_DELETE[] pStack;
	}
	else
	{
		// pop states at this node
		PopState(pStack);
	}
}

//----------------------------------------------------------------------------
void Spatial::PropagateStateFromRoot(TArray<GlobalState*>* pStack)
{
	// traverse to root to allow downward state propagation
	if (mpParent)
	{
		mpParent->PropagateStateFromRoot(pStack);
	}

	// push states onto current render state stack
	PushState(pStack);
}

//----------------------------------------------------------------------------
void Spatial::PushState(TArray<GlobalState*>* pStack)
{
	for (UInt i = 0; i < mGlobalStates.GetQuantity(); i++)
	{
		GlobalState::StateType type = mGlobalStates[i]->GetStateType();
		pStack[type].Append(mGlobalStates[i]);
	}
}

//----------------------------------------------------------------------------
void Spatial::PopState(TArray<GlobalState*>* pStack)
{
	for (UInt i = 0; i < mGlobalStates.GetQuantity(); i++)
	{
		GlobalState::StateType type = mGlobalStates[i]->GetStateType();
		pStack[type].RemoveLast();
	}
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
void Spatial::DetachEffect(Effect* pEffect)
{
	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
	{
		if (mEffects[i] == pEffect)
		{
			mEffects.RemoveAt(i);
			return;
		}
	}
}
