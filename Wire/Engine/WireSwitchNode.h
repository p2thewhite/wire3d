#pragma once
#ifndef WIRESWITCHNODE_H
#define WIRESWITCHNODE_H

#include "WireNode.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ SwitchNode: public Node
{
	WIRE_DECLARE_RTTI;

public:
    SwitchNode();
    virtual ~SwitchNode();

    enum { SN_INVALID_CHILD = -1 };

    void SetActiveChild(Int activeChild);
    Int GetActiveChild() const;
    void DisableAllChildren();

protected:
    // culling
    virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

    Int mActiveChild;
};

typedef Pointer<SwitchNode> SwitchNodePtr;
#include "WireSwitchNode.inl"

}

#endif
