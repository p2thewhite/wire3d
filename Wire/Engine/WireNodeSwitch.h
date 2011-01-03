#pragma once
#ifndef WIRESWITCHNODE_H
#define WIRESWITCHNODE_H

#include "WireNode.h"

namespace Wire
{

class NodeSwitch: public Node
{
	WIRE_DECLARE_RTTI;

public:
    NodeSwitch();
    virtual ~NodeSwitch();

    enum { SN_INVALID_CHILD = -1 };

    inline void SetActiveChild(Int activeChild);
    inline Int GetActiveChild() const;
    inline void DisableAllChildren();

protected:
    // culling
    virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

    Int mActiveChild;
};

typedef Pointer<NodeSwitch> NodeSwitchPtr;
#include "WireNodeSwitch.inl"

}

#endif
