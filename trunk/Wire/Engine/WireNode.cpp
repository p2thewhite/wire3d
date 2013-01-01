// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNode.h"

#include "WireCuller.h"
#include "WireGeometry.h"
#include "WireLight.h"
#include "WireMaterial.h"
#include "WireMesh.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Node, Spatial);

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
 		DetachChildAt(i);
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
	//     Node* pNode0 = WIRE_NEW Node;
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
	//     Node* pNode0 = WIRE_NEW Node;
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
void Node::AttachEffect(Effect* pEffect)
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
void Node::UpdateState(TArray<State*>* pStateStacks,
	TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys)
{
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		Spatial* pChild = mChildren[i];
		if (pChild)
		{
			pChild->UpdateRS(pStateStacks, pLightStack, pStateKeys);
		}
	}
}

//----------------------------------------------------------------------------
void Node::GetVisibleSet(Culler& rCuller, Bool noCull)
{
	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
	{
		// This is a global effect. Place a 'begin' marker in the visible
		// set to indicate the effect is active.
		rCuller.Insert(mEffects[i]);
	}

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

	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
	{
		// Place an 'end' marker in the visible set to indicate that the
		// global effect is inactive.
		rCuller.Insert(NULL);
	}
}

//----------------------------------------------------------------------------
Spatial* Node::GetChildByName(const String& rName) const
{
	Spatial* pFound = NULL;
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		if (mChildren[i])
		{
			if (rName == mChildren[i]->GetName())
			{
				return mChildren[i];
			}

			const Node* pNode = DynamicCast<Node>(mChildren[i]);
			if (pNode)
			{
				pFound = pNode->GetChildByName(rName);
				if (pFound)
				{
					return pFound;
				}
			}
		}
	}

	return pFound;
}

//----------------------------------------------------------------------------
void Node::GetAllChildrenByName(const String& rName, TArray<Spatial*>&
	rChildren) const
{
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		if (mChildren[i])
		{
			if (rName == mChildren[i]->GetName())
			{
				rChildren.Append(mChildren[i]);
			}

			const Node* pNode = DynamicCast<Node>(mChildren[i]);
			if (pNode)
			{
				pNode->GetAllChildrenByName(rName, rChildren);
			}
		}
	}
}

//----------------------------------------------------------------------------
void Node::GetAllChildrenByNameStartingWith(const String& rName, TArray<Spatial*>&
	rChildren) const
{
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		if (mChildren[i])
		{
			if (mChildren[i]->GetName().StartsWith(rName))
			{
				rChildren.Append(mChildren[i]);
			}

			const Node* pNode = DynamicCast<Node>(mChildren[i]);
			if (pNode)
			{
				pNode->GetAllChildrenByNameStartingWith(rName, rChildren);
			}
		}
	}
}

//----------------------------------------------------------------------------
void Node::MakeStatic(Bool forceStatic, Bool duplicateShared)
{
	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Spatial* pChild = GetChild(i);
		Node* pNode = DynamicCast<Node>(pChild);
		if (pNode)
		{
			pNode->MakeStatic(forceStatic, duplicateShared);
		}
		else
		{
			Geometry* pGeometry = DynamicCast<Geometry>(pChild);
			if (pGeometry)
			{
				pGeometry->MakeStatic(forceStatic, duplicateShared);
			}
		}
	}
}
