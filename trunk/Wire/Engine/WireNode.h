#pragma once
#ifndef WIRENODE_H
#define WIRENODE_H

#include "WireSpatial.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Node : public Spatial
{

public:
	Node();
	virtual ~Node();

	UInt GetQuantity() const;
	UInt AttachChild(Spatial* pChild);
 	Int DetachChild(Spatial* pChild);
 	SpatialPtr DetachChildAt(UInt i);
 	SpatialPtr SetChild(UInt i, Spatial* pChild);
 	SpatialPtr GetChild(UInt i);

protected:
	// geometric updates
	virtual void UpdateWorldBound();

	TArray<SpatialPtr> mChildren;
};

typedef Pointer<Node> NodePtr;
#include "WireNode.inl"

}

#endif
