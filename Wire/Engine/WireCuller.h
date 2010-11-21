#pragma once
#ifndef WIRECULLER_H
#define WIRECULLER_H

#include "WireBoundingVolume.h"
#include "WireCamera.h"
#include "WireVisibleSet.h"

namespace Wire
{

class Spatial;

class /*WIRE_ENGINE_ITEM*/ Culler
{
public:
	// Construction and destruction. The first two input parameters are used
	// to create the set of potentially visible objects. If the camera is
	// not passed to the constructor, you should set it using SetCamera before
	// calling ComputeVisibleSet.
	Culler(Int maxQuantity = VisibleSet::VS_DEFAULT_MAX_QUANTITY, Int growBy =
		VisibleSet::VS_DEFAULT_GROWBY, const Camera* pCamera = 0);
	virtual ~Culler();

	// Access to the camera, frustum copy, and potentially visible set.
	inline void SetCamera(const Camera* pCamera);
	inline const Camera* GetCamera() const;
	void SetFrustum(const Float* pFrustum);
	inline VisibleSet& GetVisibleSet();

	// This is the main function you should use for culling within a scene
	// graph. Traverse the scene and construct the potentially visible set
	// relative to the world planes.
	void ComputeVisibleSet(Spatial* pScene);

	// Compare the object's world bounding volume against all culling planes.
	Bool IsVisible(const Spatial* pSpatial) const;

	// Compare the bounding volume against the active culling planes.
	// Only Spatial calls this function (during traversal using plane state)
	Bool IsVisible(const BoundingVolume* pBV);

	// The base class behavior creates a VisibleObject from the input and
	// appends it to the end of the VisibleObject array. Derived classes
	// may override this behavior; for example, the array might be maintained
	// as a sorted array for minimizing render state changes or it might be
	// maintained as a unique list of objects for a portal system.
	inline virtual void Insert(Spatial* pObject, Effect* pGlobalEffect);

	enum { VS_MAX_PLANE_QUANTITY = 32 };
	inline void SetPlaneState(UInt planeState);
	inline UInt GetPlaneState() const;

protected:
	// The input camera has information that might be needed during the
	// culling pass over the scene.
	const Camera* mpCamera;

	// A copy of the view frustum for the input camera. This allows various
	// subsystems to change the frustum parameters during culling (for
	// example, the portal system) without affecting the camera, whose initial
	// state is needed by the renderer.
	Float mFrustum[Camera::VF_QUANTITY];

	// The world culling planes corresponding to the view frustum plus any
	// additional user-defined culling planes. The member mPlaneState
	// represents bit flags to store whether or not a plane is active in the
	// culling system. A bit of 1 means the plane is active, otherwise the
	// plane is inactive. An active plane is compared to bounding volumes,
	// whereas an inactive plane is not. This supports an efficient culling
	// of a hierarchy. For example, if a node's bounding volume is inside
	// the left plane of the view frustum, then the left plane is set to
	// inactive because the children of the node are automatically all inside
	// the left plane.
	Int mPlaneQuantity;
	Plane3F mPlanes[VS_MAX_PLANE_QUANTITY];
	UInt mPlaneState;

	// The potentially visible set for a call to GetVisibleSet.
	VisibleSet mVisible;
};

#include "WireCuller.inl"

}

#endif
