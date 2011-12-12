#pragma once
#ifndef DEMO_H
#define DEMO_H

#include "WireApplication.h"

using namespace Wire;

class Demo : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	// OnInitialize() is called by the framework after the Renderer has been
	// created. User specific initialization code goes here.
	virtual Bool OnInitialize();

	// OnIdle() is called every frame. Here we update the objects, draw them
	// and present the result on the screen.
	virtual void OnIdle();

private:
	Double mLastTime;
	Float mAngle;

	CameraPtr mspCamera;
	CullerSorting mCuller;

	SpatialPtr mspRoot;
	TArray<CameraPtr> mCameras;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
