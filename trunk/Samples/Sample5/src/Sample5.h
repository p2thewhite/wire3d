#pragma once
#ifndef SAMPLE5_H
#define SAMPLE5_H

#include "WireApplication.h"

using namespace Wire;

class Sample5 : public WIREAPPLICATION
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
	Culler mCuller;

	GeometryPtr mspCube;
	MaterialStatePtr mspMaterialState;
	LightPtr mspLight;
	LightPtr mspLight2;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample5);

#endif
