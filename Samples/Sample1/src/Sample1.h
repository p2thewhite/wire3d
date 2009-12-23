#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireEngine.h"
#include "WireMainMCR.h"
#include "WireApplication.h"

using namespace Wire;

class Sample1 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Sample1();
	virtual ~Sample1();

	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateCube();
	Geometry* CreatePyramid();

	CameraPtr mspCamera;
	GeometryPtr mspCube;
	GeometryPtr mspPyramid;
	Culler mCuller;

	Float mAngle;

	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample1);

#endif
