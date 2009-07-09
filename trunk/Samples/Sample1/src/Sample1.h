#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireEngine.h"
#include "WireGXApplication.h"

using namespace Wire;

class Sample1 : public GXApplication
{
public:
	static Bool RegisterInitialize();
	static void Initialize ();
private:
	static Bool msInitializeRegistered;

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

static Bool gsInitializeRegisteredSample1 = Sample1::RegisterInitialize();

#endif
