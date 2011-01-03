#pragma once
#ifndef SAMPLE4_H
#define SAMPLE4_H

#include "WireApplication.h"
#include "LensflareNode.h"

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
	CameraPtr mspCamera;
	Culler mCuller;

	LensflareNodePtr mspLensflare;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample4);

#endif
