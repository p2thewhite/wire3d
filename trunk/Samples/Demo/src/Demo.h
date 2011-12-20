#pragma once
#ifndef DEMO_H
#define DEMO_H

#include "WireApplication.h"

using namespace Wire;

class Demo : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Double mLastTime;

	CameraPtr mspCamera;
	CullerSorting mCuller;

	NodePtr mspRoot;
	TArray<CameraPtr> mCameras;

	//---
	TArray<Transformation*> mSplinePoints;
	Float mT;
	UInt mSplinePointIndex;

	Vector3F GetHermite(TArray<Transformation*>& rControlPoints, UInt idx,
		Float t) const;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
