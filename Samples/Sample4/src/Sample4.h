#pragma once
#ifndef SAMPLE4_H
#define SAMPLE4_H

#include "WireApplication.h"

using namespace Wire;

class Sample4 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	DLodNode* CreateLods();
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

WIRE_REGISTER_INITIALIZE(Sample4);

#endif
