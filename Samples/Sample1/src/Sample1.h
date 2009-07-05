#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireEngine.h"
#include "WireGXApplication.h"

using namespace Wire;

class Sample1 : public GXApplication
{
	typedef GXApplication Parent;

public:
	Sample1();
	virtual ~Sample1();

	virtual Int Main();

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

#endif /* SAMPLE1_H */
