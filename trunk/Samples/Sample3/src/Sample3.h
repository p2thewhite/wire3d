#pragma once
#ifndef SAMPLE3_H
#define SAMPLE3_H

#include "WireApplication.h"

using namespace Wire;

class Sample3 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	NodeDLod* CreateLods();
	Geometry* CreateGeometry(UInt shapeCount, UInt segmentCount);

	Geometry* CreatePqTorusKnot(UInt shapeCount, Float shapeRadius,
		UInt segmentCount, UInt p, UInt q);
	Texture2D* CreateTexture();

	CameraPtr mspCamera;
	Culler mCuller;

	Float mAngle;
	Double mLastTime;

	NodePtr mspRoot;
	Texture2DPtr mspTexture;
};

WIRE_REGISTER_INITIALIZE(Sample3);

#endif
