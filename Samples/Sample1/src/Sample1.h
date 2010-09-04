#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireApplication.h"

using namespace Wire;

class Sample1 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateCube();
	Texture2D* CreateTexture();

	CameraPtr mspCamera;
	GeometryPtr mspCube;
	Culler mCuller;
	CullStatePtr mspCullState;
	AlphaStatePtr mspAlphaState;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample1);

#endif
