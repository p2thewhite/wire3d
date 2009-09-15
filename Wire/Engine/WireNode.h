#pragma once
#ifndef WIRENODE_H
#define WIRENODE_H

#include "WireSpatial.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Node : public Spatial
{

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

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

	TArray<SpatialPtr> mChildren;
};

typedef Pointer<Node> NodePtr;
#include "WireNode.inl"

}

#endif
