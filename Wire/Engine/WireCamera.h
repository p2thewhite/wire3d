// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRECAMERA_H
#define WIRECAMERA_H

#include "WireMatrix34.h"
#include "WireMatrix4.h"
#include "WireVector2.h"
#include "WireObject.h"

namespace Wire
{

class Camera : public Object
{
	WIRE_DECLARE_RTTI;

public:
	Camera(Bool isPerspective = true);
	Camera(const Vector3F& location, const Vector3F& direction,
		const Vector3F& up, Bool isPerspective = true);
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

	inline Vector3F GetLocation() const;
	inline Vector3F GetDVector() const;
	inline Vector3F GetUVector() const;
	inline Vector3F GetRVector() const;

	// The camera frame is always in world coordinates.
	//   default location  E = (0, 0, 0)
	//   default direction D = (0, 0,-1)
	//   default up        U = (0, 1, 0)
	//   default right     R = (1, 0, 0)
	void SetLocation(const Vector3F& rLocation);

	void SetFrame(const Vector3F& rLocation, const Vector3F& rDVector,
		const Vector3F& rUVector, const Vector3F& rRVector);

	void LookAt(const Vector3F& rLocation, const Vector3F& rLookAt,
		const Vector3F& rUp);

	void LookAtScreenPoint(const Vector2F& rScreenPoint);

	void SetAxes(const Vector3F& rDVector, const Vector3F& rUVector,
		const Vector3F& rRVector);

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
	inline const Float* GetFrustum() const;

	// Get the individual frustum values.
	inline Float GetDMin() const;
	inline Float GetDMax() const;
	inline Float GetUMin() const;
	inline Float GetUMax() const;
	inline Float GetRMin() const;
	inline Float GetRMax() const;

	// viewport (contained in [0,1]^2)
	void SetViewport(Float left, Float right, Float top, Float bottom);
	void GetViewport(Float& rLeft, Float& rRight, Float& rTop,
		Float& rBottom);

	inline Bool IsPerspective() const;

	Matrix4F GetProjectionMatrix() const;
	Matrix4F GetViewMatrix() const;
	Vector3F ScreenToWorldPoint(const Vector2F& rScreenPoint) const;

private:
	// world coordinate frame
	Vector3F mLocation;
	Vector3F mDVector;
	Vector3F mUVector;
	Vector3F mRVector;

	// view frustum (near, far, bottom, top, left, right)
	Float mFrustum[VF_QUANTITY];

	// viewport
	Float mPortLeft;
	Float mPortRight;
	Float mPortTop;
	Float mPortBottom;

	// perspective or orthographic
	Bool mIsPerspective;
};

typedef Pointer<Camera> CameraPtr;

#include "WireCamera.inl"

}

#endif
