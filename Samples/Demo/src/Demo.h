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
	Float mAngle;
	Double mLastTime;

	CameraPtr mspCamera;
	CullerSorting mCuller;

	SpatialPtr mspRoot;

	//-----
	Node* LoadScene(const Char* pFilename);
	Image2D* LoadImage(const Char* pFilename, Bool hasMipmaps);
	Char* Load(const Char* pFilename, Int& rSize);

	Spatial* Traverse(rapidxml::xml_node<>* pXmlNode);
	Char* GetValue(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Node* ParseNode(rapidxml::xml_node<>* pXmlNode);
	Geometry* ParseLeaf(rapidxml::xml_node<>* pXmlNode);
	Mesh* ParseMesh(rapidxml::xml_node<>* pXmlNode);
	Material* ParseMaterial(rapidxml::xml_node<>* pXmlNode);
	Texture2D* ParseTexture(rapidxml::xml_node<>* pXmlNode);
	void ParseTransformation(rapidxml::xml_node<>* pXmlNode,
		Spatial* pSpatial);

	StateAlphaPtr mspAlpha;
	const Char* mpPath;
	THashTable<String, Material*> mMaterials;
	THashTable<String, Mesh*> mMeshes;
	THashTable<String, Texture2D*> mTextures;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
