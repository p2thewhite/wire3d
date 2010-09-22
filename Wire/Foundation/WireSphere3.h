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

	// Compute the smallest sphere that contains the input spheres.
	static Sphere3 MergeSpheres(const Sphere3& rSphere0, const Sphere3& rSphere1);

	Vector3<Real> Center;
	Real Radius;
};

#include "WireSphere3.inl"

typedef Sphere3<Float> Sphere3F;
typedef Sphere3<Double> Sphere3D;

}

#endif
