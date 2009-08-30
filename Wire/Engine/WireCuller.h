#pragma once
#ifndef WIRECULLER_H
#define WIRECULLER_H

#include "WireBoundingVolume.h"
#include "WireCamera.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Culler
{
public:
	// Construction and destruction. The first two input parameters are used
	// to create the set of potentially visible objects. If the camera is
	// not passed to the constructor, you should set it using SetCamera before
	// calling ComputeVisibleSet.
	Culler(Int maxQuantity = 0, Int growBy = 0, const Camera* pCamera = 0);
	virtual ~Culler();

	// Compare the object's world bounding volume against the culling planes.
	// Only Spatial calls this function.
	Bool IsVisible(const BoundingVolume* pBV);

	enum { VS_MAX_PLANE_QUANTITY = 32 };

protected:
	// The input camera has information that might be needed during the
	// culling pass over the scene.
	const Camera* mpCamera;

	// The world culling planes corresponding to the view frustum plus any
	// additional user-defined culling planes.  The member m_uiPlaneState
	// represents bit flags to store whether or not a plane is active in the
	// culling system.  A bit of 1 means the plane is active, otherwise the
	// plane is inactive.  An active plane is compared to bounding volumes,
	// whereas an inactive plane is not.  This supports an efficient culling
	// of a hierarchy.  For example, if a node's bounding volume is inside
	// the left plane of the view frustum, then the left plane is set to
	// inactive because the children of the node are automatically all inside
	// the left plane.
	Int mPlaneQuantity;
	Plane3F mPlanes[VS_MAX_PLANE_QUANTITY];
	UInt mPlaneState;
};

#include "WireCuller.inl"

}

#endif
