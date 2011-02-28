#pragma once
#ifndef SAMPLE7_H
#define SAMPLE7_H

#include "WireApplication.h"

using namespace Wire;

class Sample7 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateGeometry(UInt shapeCount, UInt segmentCount);
	Geometry* CreatePqTorusKnot(UInt shapeCount, Float shapeRadius,
		UInt segmentCount, UInt p, UInt q);

	CameraPtr mspCamera;
	GeometryPtr mspGeometry;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample7);

#endif
