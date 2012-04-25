#pragma once
#ifndef SAMPLE2_H
#define SAMPLE2_H

#include "WireApplication.h"

using namespace Wire;

class Sample2 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Sample2();

	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Node* CreateHelicopter();
	Geometry* CreateCube(ColorRGBA top, ColorRGBA bottom);

	CameraPtr mspCamera;
	Culler mCuller;

	Float mAngle;
	Double mLastTime;

	NodePtr mspRoot;
	GeometryPtr mspTopRotor;
	GeometryPtr mspRearRotor;
};

WIRE_REGISTER_INITIALIZE(Sample2);

#endif
