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

	UInt GetQuantity() const;
	UInt AttachChild(Spatial* pChild);
 	Int DetachChild(Spatial* pChild);
 	SpatialPtr DetachChildAt(UInt i);
 	SpatialPtr SetChild(UInt i, Spatial* pChild);
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
