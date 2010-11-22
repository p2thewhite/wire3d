#pragma once
#ifndef SAMPLE6_H
#define SAMPLE6_H

#include "WireApplication.h"

using namespace Wire;

class Sample6 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Geometry* CreateCube(TArray<Texture2D*>& rTextures,
		Bool useVertexColors = true);
	Texture2D* CreateTexture();
	Texture2D* CreateTexture2();

	CameraPtr mspCamera;
	NodePtr mspRoot;
	Culler mCuller;

	Float mAngle;

	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample6);

#endif
