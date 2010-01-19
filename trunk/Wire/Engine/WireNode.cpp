#include "WireNode.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Node, Spatial);

//----------------------------------------------------------------------------
Node::Node(UInt quantity, UInt growBy)
{
	mChildren.SetMaxQuantity(quantity);
	mChildren.SetGrowBy(growBy);
}

//----------------------------------------------------------------------------
Node::~Node()
{
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
 		SpatialPtr spChild = DetachChildAt(i);
 		spChild = NULL;
	}
}

//----------------------------------------------------------------------------
UInt Node::AttachChild(Spatial* pChild)
{
	// Some folks are under the impression that a node can have multiple
	// parents, the scene graph therefore being a DAG. That is not the case.
	// The parent-child relationships form a tree. This assertion is to let
	// folks know this and to warn them that a child is being kidnapped from
	// another parent. To be safe, you should really call DetachChild before
	// you reattach somewhere else with AttachChild or SetChild. If you do
	// call DetachChild first, be aware that the child might self-destruct.
	// If you want this not to happen, hang onto the child via a smart
	// pointer. For example,
	//
	//     Node* pNode0 = WM4_NEW Node;
	//     Spatial* pChild0 = <...>;
	//     pNode0->AttachChild(pChild0);  // child at index 0
	//     Node* pNode1 = <...>;
	//
	//     // This asserts because pChild0 already has a parent (pNode0).
	//     pNode1->AttachChild(pChild0);
	//
	//     // Instead do this and avoid the potential self-destruction of
	//     // pChild0).
	//     SpatialPtr spSaveChild = pNode0->GetChild(0);
	//     pNode0->DetachChild(spSaveChild);
	//     pNode1->AttachChild(spSaveChild);

	WIRE_ASSERT(pChild && !pChild->GetParent());

	pChild->SetParent(this);

	// attach child in first available slot (if any)
	UInt quantity = GetQuantity();
	for (UInt i = 0; i < quantity; i++)
	{
		if (mChildren[i] == NULL)
		{
			mChildren[i] = pChild;
			return i;
		}
	}

	// all slots used, increase array size
	mChildren.Append(pChild);
	return quantity;
}

//----------------------------------------------------------------------------
Int Node::DetachChild(Spatial* pChild)
{
	if (pChild)
	{
		// search to see if child exists
		for (UInt i = 0; i < GetQuantity(); i++)
		{
			if (mChildren[i] == pChild)
			{
				// child found, detach it
				pChild->SetParent(NULL);
				mChildren[i] = NULL;
				return i;
			}
		}
	}

	return -1;
}

//----------------------------------------------------------------------------
SpatialPtr Node::DetachChildAt(UInt i)
{
	if (i < GetQuantity())
	{
		SpatialPtr spChild = mChildren[i];
		if (spChild)
		{
			// child exists in slot, detach it
			spChild->SetParent(NULL);
			mChildren[i] = NULL;
		}

		return spChild;
	}

	return NULL;
}

//----------------------------------------------------------------------------
SpatialPtr Node::SetChild(UInt i, Spatial* pChild)
{
	// Some folks are under the impression that a node can have multiple
	// parents, the scene graph therefore being a DAG. That is not the case.
	// The parent-child relationships form a tree. This assertion is to let
	// folks know this and to warn them that a child is being kidnapped from
	// another parent. To be safe, you should really call DetachChild before
	// you reattach somewhere else with AttachChild or SetChild. If you do
	// call DetachChild first, be aware that the child might self-destruct.
	// If you want this not to happen, hang onto the child via a smart
	// pointer. For example,
	//
	//     Node* pNode0 = WM4_NEW Node;
	//     Spatial* pChild0 = <...>;
	//     pNode0->AttachChild(pChild0);  // child at index 0
	//     Node* pNode1 = <...>;
	//
	//     // This asserts because pChild0 already has a parent (pNode0).
	//     pNode1->AttachChild(pChild0);
	//
	//     // Instead do this and avoid the potential self-destruction of
	//     // pChild0).
	//     SpatialPtr spSaveChild = pNode0->GetChild(0);
	//     pNode0->DetachChild(spSaveChild);
	//     pNode1->AttachChild(spSaveChild);

	if (pChild)
	{
		WIRE_ASSERT(!pChild->GetParent());
	}

	if (i < GetQuantity())
	{
		// detach child currently in slot
		SpatialPtr spPreviousChild = mChildren[i];
		if (spPreviousChild)
		{
			spPreviousChild->SetParent(0);
		}

		// attach new child to slot
		if (pChild)
		{
			pChild->SetParent(this);
		}

		mChildren[i] = pChild;
		return spPreviousChild;
	}

	// index out of range, increase array size and attach new child
	pChild->SetParent(this);
	mChildren.Append(pChild);
	return NULL;
}

//----------------------------------------------------------------------------
SpatialPtr Node::GetChild(UInt i)
{
	if (i < GetQuantity())
	{
		return mChildren[i];
	}

	return NULL;
}

//----------------------------------------------------------------------------
void Node::UpdateWorldBound()
{
	if (!WorldBoundIsCurrent)
	{
		Bool foundFirstBound = false;
		for (UInt i = 0; i < GetQuantity(); i++)
		{
			Spatial* pChild = mChildren[i];
			if (pChild)
			{
				if (foundFirstBound)
				{
					// merge current world bound with child world bound
					WorldBound->GrowToContain(pChild->WorldBound);
				}
				else
				{
					// set world bound to first non-null child world bound
					foundFirstBound = true;
					WorldBound->CopyFrom(pChild->WorldBound);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void Node::UpdateWorldData(Double appTime)
{
	Spatial::UpdateWorldData(appTime);

	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Spatial* pChild = mChildren[i];
		if (pChild)
		{
			pChild->UpdateGS(appTime, false);
		}
	}
}

//----------------------------------------------------------------------------
void Node::UpdateState(TArray<GlobalState*>* pStack)
{
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		Spatial* pChild = mChildren[i];
		if (pChild)
		{
			pChild->UpdateRS(pStack);
		}
	}
}

//----------------------------------------------------------------------------
void Node::GetVisibleSet(Culler& rCuller, Bool noCull)
{
	// TODO: support effects

	// All Geometry objects in the subtree are added to the visible set. If
	// a global effect is active, the Geometry objects in the subtree will be
	// drawn using it.
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		Spatial* pChild = mChildren[i];
		if (pChild)
		{
			pChild->OnGetVisibleSet(rCuller, noCull);
		}
	}
}
