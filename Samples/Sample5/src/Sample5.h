#pragma once
#ifndef SAMPLE5_H
#define SAMPLE5_H

#include "WireApplication.h"

using namespace Wire;

// User applications are derived from WIREAPPLICATION, which handles
// platform dependent setup and provides virtual functions for the user
// (i.e. you) to overwrite. See WireApplication.h for details.
class Sample5 : public WIREAPPLICATION
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
	Geometry* CreateCube();
	Texture2D* CreateTexture();

	// ObjectPtr is a smart pointer. Every object derived from Wire::Object
	// has a reference count, which the smart pointer operates on to
	// determine when an object is no longer referenced. Once the reference
	// count is zero, the object is automatically deleted. Therefore NEVER
	// delete such an object manually.
	CameraPtr mspCamera;
	Culler mCuller;

	GeometryPtr mspCube;
	CullStatePtr mspCullState;
	AlphaStatePtr mspAlphaState;
	MaterialStatePtr mspMaterialState;
	LightPtr mspLight;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample5);

#endif
