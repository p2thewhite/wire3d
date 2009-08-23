#pragma once
#ifndef WIRESPHERE3_H
#define WIRESPHERE3_H

#include "WireVector3.h"

namespace Wire
{

template <class Real>
class Sphere3
{
public:
	// The sphere is represented as |X-C| = R where C is the center and R is
	// the radius.

	Sphere3();  // uninitialized
	Sphere3(const Vector3<Real>& rCenter, Real radius);
	Sphere3(const Sphere3& rSphere);

	// assignment
	Sphere3& operator= (const Sphere3& rSphere);

	Vector3<Real> mCenter;
	Real mRadius;
};

#include "WireSphere3.inl"

typedef Sphere3<Float> Sphere3F;
typedef Sphere3<Double> Sphere3D;

}

#endif
