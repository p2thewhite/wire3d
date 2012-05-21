#pragma once
#ifndef IMPORTER_H
#define IMPORTER_H

#include <vector>
#include <stdio.h>

#include "rapidxml.hpp"
#include "WireCamera.h"
#include "WireGeometry.h"
#include "WireImage2D.h"
#include "WireMaterial.h"
#include "WireMesh.h"
#include "WireNode.h"
#include "WireStateMaterial.h"
#include "WireString.h"
#include "WireTexture2D.h"

using namespace Wire;

class Importer
{
public:
	Importer(const Char* pPath = "",
		Bool materialsWithEqualNamesAreIdentical = true);

	Node* LoadSceneFromXml(const Char* pFilename, TArray<CameraPtr>*
		pCameras = NULL);
	Image2D* LoadPNG(const Char* pFilename, Bool hasMipmaps);
	static Image2D* DecodePNG(const UChar* pPngInMem, size_t pngSize,
		Bool hasMipmaps);

	struct Statistics
	{
		UInt GeometryCount;
		UInt NodeCount;
		UInt TextureCount;
		UInt MaterialCount;
		UInt VertexBufferCount;
		UInt IndexBufferCount;
	};

	const Statistics* GetStatistics();

private:
	Char* Load(const Char* pFilename, Int& rSize);
	Float* Load32(const Char* pFilename, Int& rSize, Bool isBigEndian);
	void Free32(Float* pFloats);

	void Traverse(rapidxml::xml_node<>* pXmlNode, Node* pParent);
	Char* GetValue(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Float GetFloat(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Bool GetBool(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	ColorRGB GetColorRGB(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Bool IsBigEndian(rapidxml::xml_node<>* pXmlNode);
	Buffer::UsageType GetUsageType(rapidxml::xml_node<>* pXmlNode);

	Node* ParseNode(rapidxml::xml_node<>* pXmlNode);
	Geometry* ParseLeaf(rapidxml::xml_node<>* pXmlNode);
	Mesh* ParseMesh(rapidxml::xml_node<>* pXmlNode);
	Material* ParseMaterial(rapidxml::xml_node<>* pXmlNode);
	Texture2D* ParseTexture(rapidxml::xml_node<>* pXmlNode,
		Material::BlendMode& blendMode);
	void ParseTransformation(rapidxml::xml_node<>* pXmlNode,
		Spatial* pSpatial);
	void ParseComponents(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	void ParseCamera(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	void ParseLight(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	State* ParseRenderStates(rapidxml::xml_node<>* pXmlNode);

	void UpdateGS(Spatial* pSpatial);
	void ResetStatistics();

	int decodePNG(std::vector<unsigned char>& out_image,
		unsigned long& image_width, unsigned long& image_height,
		const unsigned char* in_png, size_t in_size,
		bool convert_to_rgba32 = true);

	const Char* mpPath;
	TArray<CameraPtr>* mpCameras;

	THashTable<String, Material*> mMaterials;
	THashTable<Material*, TArray<State*> > mMaterialStates;
	THashTable<String, Mesh*> mMeshes;
	THashTable<String, Texture2D*> mTextures;

	Bool mMaterialsWithEqualNamesAreIdentical;
	Statistics mStatistics;
};

#endif
