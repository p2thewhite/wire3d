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

private:
	Matrix34f mWorldToCamera;
};

typedef Pointer<Camera> CameraPtr;

}

#endif /* WIRECAMERA_H */
