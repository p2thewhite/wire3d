#pragma once
#ifndef IMPORTER_H
#define IMPORTER_H

#include <vector>
#include <stdio.h>

#include "rapidxml.hpp"
#include "WireCamera.h"
#include "WireColorRGBA.h"
#include "WireGeometry.h"
#include "WireNode.h"
#include "WireString.h"
#include "WireText.h"
#include "WireVector2.h"
#include "WireVertexBuffer.h"
#include "WireIndexBuffer.h"

#include "Collider.h"
#include "btBulletDynamicsCommon.h"

using namespace Wire;

struct FT_FaceRec_;

class Importer
{

public:
	struct Statistics
	{
		UInt GeometryCount;
		UInt NodeCount;
		UInt TextureCount;
		UInt MaterialCount;
		UInt VertexBufferCount;
		UInt IndexBufferCount;
		UInt ColliderCount;
	};

	Importer(const Char* pPath = "", Bool materialsWithEqualNamesAreIdentical = true,
		Bool prepareForStaticBatching = true);

	static Image2D* DecodePNG(const UChar* pPngInMem, size_t pngSize, Bool hasMipmaps);
	static Image2D* LoadPNG(const Char* pFilename, Bool hasMipmaps);
	static Texture2D* LoadTexture2D(const Char* pFilename, Bool hasMipmaps);
	static Text* CreateText(const Char* pFilename, UInt width, UInt height, UInt maxLength = 4000);

	Node* LoadSceneFromXml(const Char* pFilename, TArray<CameraPtr>* pCameras = NULL,
		btDynamicsWorld* pPhysicsWorld = NULL);
	const Statistics* GetStatistics();

private:
	const Char* mpPath;
	TArray<CameraPtr>* mpCameras;
	THashTable<String, Material*> mMaterials;
	THashTable<Material*, TArray<State*> > mMaterialStates;
	THashTable<String, Mesh*> mMeshes;
	THashTable<String, Texture2D*> mTextures;
	TArray<Spatial*> mStaticSpatials;
	TArray<Collider*> mColliders;
	Bool mMaterialsWithEqualNamesAreIdentical;
	Bool mPrepareForStaticBatching;
	Statistics mStatistics;

	static Char* Load(const Char* pFilename, Int& rSize);
	static void InitializeStaticSpatials(TArray<Spatial*>& rSpatials, Bool prepareForStaticBatching);
	static void RegisterColliders(TArray<Collider*>& rColliders, btDynamicsWorld* pPhysicsWorld);

	Float* Load32(const Char* pFilename, Int& rSize, Bool isBigEndian);
	void Free32(Float* pFloats);
	void Traverse(rapidxml::xml_node<>* pXmlNode, Node* pParent);
	Char* GetValue(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Bool HasValue(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Float GetFloat(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Bool GetBool(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	ColorRGB GetColorRGB(rapidxml::xml_node<>* pXmlNode, const Char* pName,	Bool& rHasValue);
	ColorRGBA GetColorRGBA(rapidxml::xml_node<>* pXmlNode, const Char* pName, Bool& rHasValue);
	Bool IsBigEndian(rapidxml::xml_node<>* pXmlNode);
	Buffer::UsageType GetUsageType(rapidxml::xml_node<>* pXmlNode);
	Bool Is(const Char*, const Char*);
	Node* ParseNode(rapidxml::xml_node<>* pXmlNode);
	Geometry* ParseLeaf(rapidxml::xml_node<>* pXmlNode);
	Text* ParseText(rapidxml::xml_node<>* pXmlNode);
	Mesh* ParseMesh(rapidxml::xml_node<>* pXmlNode);
	IndexBuffer* LoadIndexBufferFromFile(Char* pFileName, Bool isIndexBufferBigEndian, Buffer::UsageType indexBufferUsage);
	VertexBuffer* LoadVertexBufferFromFiles(Char* pFileName, Bool isVertexBufferBigEndian, Buffer::UsageType vertexBufferUsage, Char* pNormalsName, Bool isNormalsBigEndian, Char* pColorsName, Bool isColorsBigEndian, TArray<Char*> &uvSetNames, TArray<Bool> uvBigEndian);
	Material* ParseMaterial(rapidxml::xml_node<>* pXmlNode);
	Texture2D* ParseTexture(rapidxml::xml_node<>* pXmlNode, Material::BlendMode& blendMode);
	void ParseTransformation(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	void ParseComponents(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	void ParseCamera(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	void ParseLight(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	void ParseCollider(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	State* ParseRenderStates(rapidxml::xml_node<>* pXmlNode);
	void ParseTransformationAndComponents(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial);
	void UpdateGS(Spatial* pSpatial);
	void ResetStatistics();
	int DecodePNG(std::vector<unsigned char>& rOutImage, unsigned long& imageWidth, unsigned long& imageHeight, const unsigned char* pInPng, size_t inSize, bool convertToRGBA32 = true);

};

#endif
