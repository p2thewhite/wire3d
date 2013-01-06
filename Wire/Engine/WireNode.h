// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRENODE_H
#define WIRENODE_H

#include "WireEffect.h"
#include "WireSpatial.h"

namespace Wire
{

class Node : public Spatial
{
	WIRE_DECLARE_RTTI;

public:
	Node(UInt quantity = 0, UInt growBy = 1);
	virtual ~Node();

	// This is the current number of elements in the child array. These
	// elements are not all guaranteed to be non-null. Thus, when you
	// iterate over the array and access children with GetChild(...), you
	// should test the return pointer to be non-null before dereferencing it.
	inline UInt GetQuantity() const;

	// Attach a child to this node. If the function succeeds, the return
	// value is the index i of the array where the child was stored, in which
	// case 0 <= i < GetQuantity(). The first available empty slot of the
	// child array is used for storage. If all slots are filled, the child
	// is appended to the array (potentially causing a reallocation of the
	// array).
	UInt AttachChild(Spatial* pChild);

	// Detach a child from this node. If the 'child' is non-null and in the
	// array, the return value is the index in the array that had stored the
	// child. Otherwise, the function returns -1.
 	Int DetachChild(Spatial* pChild);
 
	// Detach a child from this node. If i < GetQuantity(), the
	// return value is the child at index i; otherwise, the function returns
	// null.
	SpatialPtr DetachChildAt(UInt i);

	// The same comments for AttachChild apply here regarding the inability
	// to have multiple parents. If i < GetQuantity(), the function
	// succeeds and returns i. If i is out of range, the function *still*
	// succeeds, appending the child to the end of the array.  The return
	// value is the previous child stored at index i.
	SpatialPtr SetChild(UInt i, Spatial* pChild);

	// Get the child at the specified index. If i < GetQuantity(),
	// the function succeeds and returns the child at that index. Keep in mind
	// that child[i] could very well be null. If i is out of range, the
	// function returns null.
 	SpatialPtr GetChild(UInt i);

	// returns the first (top/down, left-to-right) Spatial using that name
	Spatial* GetChildByName(const String& rName) const;

	void GetAllChildrenByName(const String& rName, TArray<Spatial*>&
		rChildren) const;

	void GetAllChildrenByNameStartingWith(const String& rName, TArray<Spatial*>&
		rChildren) const;

	// geometric update
	virtual void UpdateWorldBound();

	// Bind/Unbind all renderer related resources of the subgraph
	virtual void Bind(Renderer* pRenderer);
	virtual void Unbind(Renderer* pRenderer);

	// effect state
	inline UInt GetEffectQuantity() const;
	inline Effect* GetEffect(UInt i = 0) const;
	void AttachEffect(Effect* pEffect);
	inline void DetachEffect(Effect* pEffect);
	inline void DetachAllEffects();

	// Traverse the child objects and call their MakeStatic()
	virtual void MakeStatic(Bool forceStatic = false,
		Bool duplicateShared = true);

protected:
	// geometric update
	virtual void UpdateWorldData(Double appTime);

	// render state updates
	virtual void UpdateState(TArray<State*>* pStateStacks,
		TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

	TArray<SpatialPtr> mChildren;

	// Effect state. If the effect is attached to a Geometry object, it
	// applies to that object alone. If the effect is attached to a Node
	// object, it applies to all Geometry objects in the subtree rooted at
	// the Node.
	TArray<Pointer<Effect> > mEffects;
};

typedef Pointer<Node> NodePtr;
#include "WireNode.inl"

}

#endif
