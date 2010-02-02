#pragma once
#ifndef WIREBOUNDINGVOLUME_H
#define WIREBOUNDINGVOLUME_H

#include "WireObject.h"
#include "WirePlane3.h"
#include "WireTransformation.h"
#include "WireVector3.h"
#include "WireVertexBuffer.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ BoundingVolume : public Object
{
	WIRE_DECLARE_RTTI;

public:
	// abstract base class
	virtual ~BoundingVolume();

	// One of the derived classes has the responsibility for implementing
	// this factory function. Our default is the WireSphereBV class, but
	// you may change it to another.
	static BoundingVolume* Create();

	// all bounding volumes must define a center and radius
	virtual void SetCenter(const Vector3F& rCenter) = 0;
	virtual void SetRadius(Float radius) = 0;
	virtual Vector3F GetCenter() const = 0;
	virtual Float GetRadius() const = 0;

	// Compute a bounding volume that contains all the points.
	virtual void ComputeFromData(const VertexBuffer* pVBuffer) = 0;

	// Transform the bounding volume (model-to-world conversion).
	virtual void TransformBy(const Transformation& rTransform,
		BoundingVolume* pResult) = 0;

	// Determine if the bounding volume is one side of the plane, the other
	// side, or straddles the plane. If it is on the positive side (the
	// side to which the normal points), the return value is +1. If it is
	// on the negative side, the return value is -1. If it straddles the
	// plane, the return value is 0.
	virtual Int WhichSide(const Plane3F& rPlane) const = 0;

	// Make a copy of the bounding volume.
	virtual void CopyFrom(const BoundingVolume* pInput) = 0;

	// Change the current bounding volume so that it contains the input
	// bounding volume as well as its old bounding volume.
	virtual void GrowToContain(const BoundingVolume* pInput) = 0;

protected:
	BoundingVolume();
};

typedef Pointer<BoundingVolume> BoundingVolumePtr;

}

#endif
