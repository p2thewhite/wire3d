#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireEngine.h"
#include "WireApplication.h"

using namespace Wire;

class RenderTest : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	RenderTest();
	virtual ~RenderTest();

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

WIRE_REGISTER_INITIALIZE(RenderTest);

#endif
