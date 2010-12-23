#pragma once
#ifndef WIRENODE_H
#define WIRENODE_H

#include "WireSpatial.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Node : public Spatial
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
 
	// Detach a child from this node. If 0 <= i < GetQuantity(), the
	// return value is the child at index i; otherwise, the function returns
	// null.
	SpatialPtr DetachChildAt(UInt i);

	// The same comments for AttachChild apply here regarding the inability
	// to have multiple parents. If 0 <= i < GetQuantity(), the function
	// succeeds and returns i. If i is out of range, the function *still*
	// succeeds, appending the child to the end of the array.  The return
	// value is the previous child stored at index i.
	SpatialPtr SetChild(UInt i, Spatial* pChild);

	// Get the child at the specified index. If 0 <= i < GetQuantity(),
	// the function succeeds and returns the child at that index. Keep in mind
	// that child[i] could very well be null. If i is out of range, the
	// function returns null.
 	SpatialPtr GetChild(UInt i);

protected:
	// geometric updates
	virtual void UpdateWorldData(Double appTime);
	virtual void UpdateWorldBound();

	// render state updates
	virtual void UpdateState(TArray<GlobalState*>* pGStack,
		TArray<Light*>* pLStack);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

	TArray<SpatialPtr> mChildren;
};

typedef Pointer<Node> NodePtr;
#include "WireNode.inl"

}

#endif
