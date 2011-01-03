#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireApplication.h"

using namespace Wire;

// User applications are derived from WIREAPPLICATION, which handles
// platform dependent setup and provides virtual functions for the user
// (i.e. you) to override. See WireApplication.h for details.
class Sample1 : public WIREAPPLICATION
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
	// count is zero, the object is deleted automatically. Therefore NEVER
	// delete such an object manually.
	//
	// The following coding style is used throughout the framework:
	// 'pObject' = pointer to Object
	// 'rObject' = reference to Object
	// 'spObject' = smart pointer to Object
	// 'mObject' = Object is a member variable of the class
	// 'mpObject' = member pointer to Object
	// 'mspObject' = member smart pointer to Object
	CameraPtr mspCamera;
	Culler mCuller;

	GeometryPtr mspCube;
	TextureEffectPtr mspTextureEffect;

	LightPtr mspLight;
	StateMaterialPtr mspMaterial;

	StateCullPtr mspCull;
	StateAlphaPtr mspAlpha;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample1);

#endif
