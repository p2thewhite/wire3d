#pragma once
#ifndef SAMPLE2_H
#define SAMPLE2_H

#include "WireEngine.h"
#include "WireMainMCR.h"
#include "WireApplication.h"

using namespace Wire;

class Sample2 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Sample2();
	virtual ~Sample2();

	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateCube();
	Geometry* CreatePyramid();

	CameraPtr mspCamera;
	Culler mCuller;
	NodePtr mspRoot;

	Float mAngle;
};

WIRE_REGISTER_INITIALIZE(Sample2);

#endif
