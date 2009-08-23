#pragma once
#ifndef WIRESPHEREBV_H
#define WIRESPHEREBV_H

#include "../Foundation/WireSphere3.h"
#include "WireBoundingVolume.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ SphereBV : public BoundingVolume
{

public:
	SphereBV();
	SphereBV(const Sphere3F& rSphere);
	virtual ~SphereBV();

	// all bounding volumes must define a center and radius
	virtual void SetCenter(const Vector3F& rCenter);
	virtual void SetRadius(Float radius);
	virtual Vector3F GetCenter() const;
	virtual Float GetRadius() const;

	Sphere3F& Sphere();
	const Sphere3F& GetSphere() const;

	// Compute a sphere that contains all the points.
	virtual void ComputeFromData(const VertexBuffer* pVBuffer);

	// Transform the sphere (model-to-world conversion).
	virtual void TransformBy(const Transformation& rTransform,
		BoundingVolume* pResult);

protected:
	Sphere3F mSphere;
};

typedef Pointer<SphereBV> SphereBVPtr;
#include "WireSphereBV.inl"

}

#endif
