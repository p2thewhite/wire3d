#pragma once
#ifndef SPLINECAMERA_H
#define SPLINECAMERA_H

#include "WireCamera.h"
#include "WireController.h"
#include "WireNode.h"
#include "WireQuaternion.h"

using namespace Wire;

class SplineCamera : public Controller
{
public:
	SplineCamera(Node* pSplineRoot, Camera* pCamera);

	virtual Bool Update(Double appTime);

private:
	Vector3F GetHermite(TArray<Transformation*>& rControlPoints, UInt idx,
		Float t);
	QuaternionF GetSquad(TArray<Transformation*>& rControlPoints, UInt idx,
		Float t);

	TArray<Transformation*> mSplinePoints;
	CameraPtr mspCamera;
	Float mT;
	UInt mSplinePointIndex;
};

#endif
