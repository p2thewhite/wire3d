#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireEngine.h"
#include "WireApplication.h"

using namespace Wire;

class RenderTest : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	RenderTest();
	virtual ~RenderTest();

	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateCube();
	Geometry* CreatePyramid();
	Texture2D* CreateTexture();

	CameraPtr mspCamera;
	GeometryPtr mspCube;
	GeometryPtr mspPyramid;
	Culler mCuller;

	Float mAngle;

	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(RenderTest);

#endif
