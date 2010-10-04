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
	Sample4();

	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateCube();
	Texture2D* CreateTexture();

	Float SmoothStep(Float a, Float b, Float x);
	void DrawParticle(Float* const pDst, Float fx, Float fy, UInt width);

	CameraPtr mspCamera;
	Culler mCuller;

	GeometryPtr mspCube;
	CullStatePtr mspCullState;
	AlphaStatePtr mspAlphaState;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample4);

#endif
