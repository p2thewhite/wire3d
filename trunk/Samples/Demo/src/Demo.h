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

	SpatialPtr mspRoot;

	//-----
	Node* LoadScene(const Char* pFilename);
	Spatial* Traverse(rapidxml::xml_node<>* pXmlNode);

	Image2D* LoadImage(const Char* pFilename, Bool hasMipmaps);

	Node* ParseNode(rapidxml::xml_node<>* pXmlNode);
	Geometry* ParseLeaf(rapidxml::xml_node<>* pXmlNode);
	Mesh* ParseMesh(rapidxml::xml_node<>* pXmlNode);
	Material* ParseMaterial(rapidxml::xml_node<>* pXmlNode);
	Texture2D* ParseTexture(rapidxml::xml_node<>* pXmlNode);

	Char* mpPath;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
