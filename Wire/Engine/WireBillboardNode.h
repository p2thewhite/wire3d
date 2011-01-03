#pragma once
#ifndef WIREBILLBOARDNODE_H
#define WIREBILLBOARDNODE_H

#include "WireNode.h"

namespace Wire
{

class Camera;

class NodeBillboard : public Node
{
	WIRE_DECLARE_RTTI;

public:
    // The model space of the billboard has an up vector of (0,1,0) that is
    // chosen to be the billboard's axis of rotation.

    // Construction and destruction.
    NodeBillboard(Camera* pCamera = NULL);
    virtual ~NodeBillboard();

    // the camera to which the billboard is aligned
    inline void AlignTo(Camera* pCamera);

protected:
    // geometric updates
    virtual void UpdateWorldData(Double appTime);

    Pointer<Camera> mspCamera;
};

typedef Pointer<NodeBillboard> NodeBillboardPtr;

#include "WireBillboardNode.inl"

}

#endif
