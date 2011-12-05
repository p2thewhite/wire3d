#pragma once
#ifndef DEMO_H
#define DEMO_H

#include "WireApplication.h"
#include "rapidxml.hpp"

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
	Node* LoadScene(const Char* pFilename);
	Image2D* LoadImage(const Char* pFilename, Bool hasAlpha, Bool hasMipmaps);
	void Traverse(rapidxml::xml_node<>* pXmlNode);

	Geometry* CreateCube();
	Texture2D* CreateTexture();

	CameraPtr mspCamera;
	Culler mCuller;

	GeometryPtr mspCube;

	LightPtr mspLight;
	StateMaterialPtr mspMaterial;

	StateCullPtr mspCull;
	StateAlphaPtr mspAlpha;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
