#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireEngine.h"
#include "WireGXApplication.h"
#include "WireMainMCR.h"

using namespace Wire;

class Sample1 : public GXApplication
{
	WIRE_DECLARE_INITIALIZE;

	typedef GXApplication Parent;

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

	Float mRtri;
	Float mRquad;
	Float mAngle;
};

WIRE_REGISTER_INITIALIZE(Sample1);

#endif
