#pragma once
#ifndef IMPORTER_H
#define IMPORTER_H

#include <vector>
#include <stdio.h>

#include "rapidxml.hpp"
#include "WireGeometry.h"
#include "WireImage2D.h"
#include "WireMaterial.h"
#include "WireMesh.h"
#include "WireNode.h"
#include "WireStateAlpha.h"
#include "WireString.h"
#include "WireTexture2D.h"

using namespace Wire;

class Importer
{
public:
	Importer(const Char* pPath = "");

	Node* LoadSceneFromXml(const Char* pFilename);
	Image2D* LoadPNG(const Char* pFilename, Bool hasMipmaps);

private:
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

	int decodePNG(std::vector<unsigned char>& out_image,
		unsigned long& image_width, unsigned long& image_height,
		const unsigned char* in_png, size_t in_size,
		bool convert_to_rgba32 = true);

	StateAlphaPtr mspAlpha;
	const Char* mpPath;
	THashTable<String, Material*> mMaterials;
	THashTable<String, Mesh*> mMeshes;
	THashTable<String, Texture2D*> mTextures;
};

#endif
