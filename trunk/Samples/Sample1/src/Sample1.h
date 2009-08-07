#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireEngine.h"
#include "WireMainMCR.h"

// TODO: clean that ugly mess.
#ifdef WIRE_WIN
#include "WireDXApplication.h"
#else
#include "WireGXApplication.h"
#endif

using namespace Wire;

#ifdef WIRE_WIN
class Sample1 : public DXApplication
#else
class Sample1 : public GXApplication
#endif
{
	WIRE_DECLARE_INITIALIZE;

#ifdef WIRE_WIN
	typedef DXApplication Parent;
#else
	typedef GXApplication Parent;
#endif

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
