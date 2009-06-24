#pragma once
#ifndef WIRECAMERA_H
#define WIRECAMERA_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Camera : public Object
{
public:
	Camera();
	virtual ~Camera();

	void LookAt(const Vector3f& cameraPosition, const Vector3f& cameraUp,
		const Vector3f& target);

	// Set a symmetric view frustum (umin = -umax, rmin = -rmax) 
	// using a field of view in the "up" direction and an aspect ratio
	// "width/height".  This call is the equivalent of gluPerspective in
	// OpenGL and MTXPerspective in GX.  As such, the field of view in 
	// this function must be specified in degrees and be in the
	// interval (0, 180).
	void SetFrustum(Float upFovDegrees, Float aspectRatio, Float dMin,
		Float dMax);

	Matrix34f& GetView() { return mView; }	

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

	Matrix34f mView;  // Transforms world space to camera space.

	Float mFrustum[VF_QUANTITY];

};

typedef Pointer<Camera> CameraPtr;

}

#endif /* WIRECAMERA_H */
