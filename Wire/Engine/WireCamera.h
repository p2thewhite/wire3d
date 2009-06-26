#pragma once
#ifndef WIRECAMERA_H
#define WIRECAMERA_H

#include "WireObject.h"
#include "../Foundation/WireMatrix34.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Camera : public Object
{
public:
	Camera();
	virtual ~Camera();

	void LookAt(const Vector3f& cameraPosition, const Vector3f& cameraUp,
		const Vector3f& target);

	Matrix34f& GetView();

	// Set the view frustum.  The interval [rmin, rmax] is measured in the
	// right direction R.  These are the "left" and "right" frustum values.
	// The interval [umin, umax] is measured in the up direction U.  These
	// are the "bottom" and "top" values.  The interval [dmin, dmax] is
	// measured in the view direction D.  These are the "near" and "far"
	// values.
	void SetFrustum(Float rMin, Float rMax, Float uMin, Float uMax,
		Float dMin, Float dMax);

	// Set a symmetric view frustum (umin = -umax, rmin = -rmax) 
	// using a field of view in the "up" direction and an aspect ratio
	// "width/height".  This call is the equivalent of gluPerspective in
	// OpenGL and MTXPerspective in GX.  As such, the field of view in 
	// this function must be specified in degrees and be in the
	// interval (0, 180).
	void SetFrustum(Float upFovDegrees, Float aspectRatio, Float dMin,
		Float dMax);

	void GetFrustum(Float& rRMin, Float& rRMax, Float& rUMin, Float& rUMax,
		Float& rDMin, Float& rDMax) const;

	// Get the parameters for a symmetric view frustum.  The return value is
	// 'true' if the current frustum is symmetric, in which case the output
	// parameters are valid.
	Bool GetFrustum(Float& rUpFovDegrees, Float& rAspectRatio,
		Float& rDMin, Float& rDMax) const;

	// Get the individual frustum values.
	Float GetDMin() const;
	Float GetDMax() const;
	Float GetUMin() const;
	Float GetUMax() const;
	Float GetRMin() const;
	Float GetRMax() const;

private:
	enum
	{
		VF_DMIN     = 0,  // near
		VF_DMAX     = 1,  // far
		VF_UMIN     = 2,  // bottom
		VF_UMAX     = 3,  // top
		VF_RMIN     = 4,  // left
		VF_RMAX     = 5,  // right
		VF_QUANTITY = 6
	};

	// Transforms world space to camera space.
	Matrix34f mView;  

	Float mFrustum[VF_QUANTITY];

};

typedef Pointer<Camera> CameraPtr;

#include "WireCamera.inl"

}

#endif /* WIRECAMERA_H */
