#pragma once
#ifndef WIRECAMERA_H
#define WIRECAMERA_H

#include "WireMatrix34.h"
#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Camera : public Object
{
	WIRE_DECLARE_RTTI;

public:
	Camera();
	virtual ~Camera();

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

	Vector3F GetLocation() const;
	Vector3F GetDVector() const;
	Vector3F GetUVector() const;
	Vector3F GetRVector() const;

	void SetAxes(const Vector3F& rDVector, const Vector3F& rUVector,
		const Vector3F& rRVector);

	// The camera frame is always in world coordinates.
	//   default location  E = (0, 0, 0)
	//   default direction D = (0, 0,-1)
	//   default up        U = (0, 1, 0)
	//   default right     R = (1, 0, 0)
	void SetFrame(const Vector3F& rLocation, const Vector3F& rDVector,
		const Vector3F& rUVector, const Vector3F& rRVector);

	// Set the view frustum. The interval [rmin, rmax] is measured in the
	// right direction R. These are the "left" and "right" frustum values.
	// The interval [umin, umax] is measured in the up direction U.  These
	// are the "bottom" and "top" values. The interval [dmin, dmax] is
	// measured in the view direction D. These are the "near" and "far"
	// values.
	void SetFrustum(Float rMin, Float rMax, Float uMin, Float uMax,
		Float dMin, Float dMax);

	// Set a symmetric view frustum (umin = -umax, rmin = -rmax) 
	// using a field of view in the "up" direction and an aspect ratio
	// "width/height". This call is the equivalent of gluPerspective in
	// OpenGL and MTXPerspective in GX. As such, the field of view in 
	// this function must be specified in degrees and be in the
	// interval (0, 180).
	void SetFrustum(Float upFovDegrees, Float aspectRatio, Float dMin,
		Float dMax);

	void GetFrustum(Float& rRMin, Float& rRMax, Float& rUMin, Float& rUMax,
		Float& rDMin, Float& rDMax) const;

	// Get the parameters for a symmetric view frustum. The return value is
	// 'true' if the current frustum is symmetric, in which case the output
	// parameters are valid.
	Bool GetFrustum(Float& rUpFovDegrees, Float& rAspectRatio,
		Float& rDMin, Float& rDMax) const;

	// Get all the view frustum values simultaneously.
	const Float* GetFrustum() const;

	// Get the individual frustum values.
	Float GetDMin() const;
	Float GetDMax() const;
	Float GetUMin() const;
	Float GetUMax() const;
	Float GetRMin() const;
	Float GetRMax() const;

private:
	// world coordinate frame
	Vector3F mLocation;
	Vector3F mDVector;
	Vector3F mUVector;
	Vector3F mRVector;

	Float mFrustum[VF_QUANTITY];
};

typedef Pointer<Camera> CameraPtr;

#include "WireCamera.inl"

}

#endif
