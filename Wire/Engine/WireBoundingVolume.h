#pragma once
#ifndef WIREBOUNDINGVOLUME_H
#define WIREBOUNDINGVOLUME_H

#include "WireObject.h"
#include "../Foundation/WireVector3.h"
#include "WireTransformation.h"
#include "WireVertexBuffer.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ BoundingVolume : public Object
{

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
	virtual void TransformBy(/*const*/ Transformation& rTransform,
		BoundingVolume* pResult) = 0; // TODO: fix for VC

protected:
	BoundingVolume();
};

typedef Pointer<BoundingVolume> BoundingVolumePtr;

}

#endif
