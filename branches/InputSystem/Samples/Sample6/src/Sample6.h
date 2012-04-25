#pragma once
#ifndef SAMPLE6_H
#define SAMPLE6_H

#include "WireApplication.h"
#include "LensflareNode.h"

using namespace Wire;

class Sample6 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateSun();
	Geometry* CreateTerrain();

	CameraPtr mspCamera;
	Culler mCuller;

	NodePtr mspRoot;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample6);

#endif
