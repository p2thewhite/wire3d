#pragma once
#ifndef IMPORTER_H
#define IMPORTER_H

#include <vector>
#include <stdio.h>

#include "rapidxml.hpp"
#include "WireCamera.h"
#include "WireColorRGBA.h"
#include "WireGeometry.h"
#include "WireMesh.h"
#include "WireNode.h"
#include "WireNodeSkybox.h"
#include "WireString.h"
#include "WireText.h"
#include "WireVector2.h"
#include "WireVertexBuffer.h"
#include "WireIndexBuffer.h"

#include "Collider.h"
#include "btBulletDynamicsCommon.h"

struct FT_FaceRec_;

class Importer
{

public:
	struct Options
	{
		Options()
			:
			AssetsWithEqualNamesAreIdentical(true),
			PrepareSceneForStaticBatching(false),
			DuplicateSharedMeshesWhenPreparingSceneForStaticBatching(false),
			CreateInterleavedVertexBuffers(true)
			{}
			 
		Bool AssetsWithEqualNamesAreIdentical;
		Bool PrepareSceneForStaticBatching;
		Bool DuplicateSharedMeshesWhenPreparingSceneForStaticBatching;
		Bool CreateInterleavedVertexBuffers;
	};

	struct Statistics
	{
		UInt GeometryCount;
		UInt NodeCount;
		UInt TextureCount;
		UInt MaterialCount;
		UInt MeshCount;
		UInt VertexBufferCount;
		UInt IndexBufferCount;
		UInt ColliderCount;
	};

	Importer(const Char* pPath = "", Options* pOptions = NULL);

	Wire::Node* LoadSceneFromXml(const Char* pFilename,
		Wire::TArray<Wire::CameraPtr>* pCameras = NULL,
		btDynamicsWorld* pPhysicsWorld = NULL);

	static Wire::Image2D* DecodePNG(const UChar* pPngInMem, size_t pngSize, Bool hasMipmaps,
		Wire::Buffer::UsageType usage = Wire::Buffer::UT_STATIC);
	static Wire::Image2D* LoadPNG(const Char* pFilename, Bool hasMipmaps,
		Wire::Buffer::UsageType usage = Wire::Buffer::UT_STATIC);
	static Wire::Texture2D* LoadTexture2D(const Char* pFilename, Bool hasMipmaps,
		Wire::Buffer::UsageType usage = Wire::Buffer::UT_STATIC);
	static Wire::Text* CreateText(const Char* pFilename, UInt width,
		UInt height, UInt maxLength = 4000);

	const Statistics* GetStatistics();

private:
	static Char* Load(const Char* pFilename, Int& rSize);
	static void InitializeStaticSpatials(Wire::TArray<Wire::SpatialPtr>&
		rSpatials, Bool prepareSceneForStaticBatching,
		Bool duplicateSharedMeshesWhenPreparingSceneForStaticBatching);
	static void RegisterColliders(Wire::TArray<Collider*>& rColliders,
		btDynamicsWorld* pPhysicsWorld);

	Float* Load32(const Char* pFilename, Int& rSize, Bool isBigEndian);
	UShort* Load16(const Char* pFilename, Int& rSize, Bool isBigEndian);
	void Free32(Float* pFloats);

	void Traverse(rapidxml::xml_node<>* pXmlNode, Wire::Node* pParent);
	Char* GetValue(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Bool HasValue(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Float GetFloat(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	UInt GetUInt(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Bool GetBool(rapidxml::xml_node<>* pXmlNode, const Char* pName);
	Wire::ColorRGB GetColorRGB(rapidxml::xml_node<>* pXmlNode,
		const Char* pName, Bool& rHasValue);
	Wire::ColorRGBA GetColorRGBA(rapidxml::xml_node<>* pXmlNode,
		const Char* pName, Bool& rHasValue);
	Bool IsBigEndian(rapidxml::xml_node<>* pXmlNode);
	Bool Is16Bit(rapidxml::xml_node<>* pXmlNode);
	Wire::Buffer::UsageType GetUsageType(rapidxml::xml_node<>* pXmlNode);
	Bool Is(const Char*, const Char*);

	void ParseAssets(rapidxml::xml_node<>* pXmlNode);
	Wire::Node* ParseNode(rapidxml::xml_node<>* pXmlNode);
	Wire::Geometry* ParseLeaf(rapidxml::xml_node<>* pXmlNode);
	Wire::Text* ParseText(rapidxml::xml_node<>* pXmlNode);
	Wire::NodeSkybox* ParseSkybox(rapidxml::xml_node<>* pXmlNode);
	Wire::Texture2D* ParseSkyboxTexture(const Char* pName, rapidxml::xml_node<>* pXmlNode);
	Wire::Mesh* ParseMesh(rapidxml::xml_node<>* pXmlNode, UInt subMeshIndex = 0);
	Wire::VertexBuffer* ParseVertexBuffer(rapidxml::xml_node<>* pXmlNode);
	void ParseVertexBuffers(rapidxml::xml_node<>* pXmlNode, Wire::TArray<
		Wire::VertexBuffer*>& rVertexBuffers);
	Wire::IndexBuffer* ParseIndexBuffer(rapidxml::xml_node<>* pXmlNode);
	Wire::Material* ParseMaterial(rapidxml::xml_node<>* pXmlNode);
	Wire::Texture2D* ParseTexture(rapidxml::xml_node<>* pXmlNode, Wire::Material::BlendMode& blendMode);
	void ParseTransformation(rapidxml::xml_node<>* pXmlNode, Wire::Spatial* pSpatial);
	void ParseComponent(rapidxml::xml_node<>* pXmlNode, Wire::Spatial* pSpatial);
	void ParseComponents(rapidxml::xml_node<>* pXmlNode, Wire::Spatial* pSpatial);
	void ParseCamera(rapidxml::xml_node<>* pXmlNode, Wire::Spatial* pSpatial);
	Wire::Light* ParseLight(rapidxml::xml_node<>* pXmlNode);
	void ParseCollider(rapidxml::xml_node<>* pXmlNode, Wire::Spatial* pSpatial);
	Wire::State* ParseRenderStates(rapidxml::xml_node<>* pXmlNode);
	void ParseTransformationAndComponents(rapidxml::xml_node<>* pXmlNode,
		Wire::Spatial* pSpatial);

	void UpdateGS(Wire::Spatial* pSpatial);
	void ResetStatistics();

	Wire::IndexBuffer* LoadIndexBufferFromFile(Char* pFileName, Bool isIndexBufferBigEndian,
		Wire::Buffer::UsageType indexBufferUsage, Bool is16Bit);
	Wire::VertexBuffer* LoadVertexBufferFromFiles(Char* pFileName, Bool isVertexBufferBigEndian,
		Wire::Buffer::UsageType vertexBufferUsage, Char* pNormalsName, Bool isNormalsBigEndian,
		Char* pColorsName, Bool isColorsBigEndian, Wire::TArray<Char*>& rUvSetNames,
		Wire::TArray<Bool>& rUvBigEndian);
	Wire::VertexBuffer* LoadVertexBuffer(Char* pFileName, Bool isBigEndian,
		Wire::Buffer::UsageType usage, Wire::VertexAttributes& rAttributes);

	const Char* mpPath;
	Wire::TArray<Wire::CameraPtr>* mpCameras;

	Wire::THashTable<Wire::String, Wire::MaterialPtr> mMaterials;
	Wire::THashTable<Wire::Material*, Wire::TArray<Wire::StatePtr> > mMaterialStates;
	Wire::THashTable<Wire::String, Wire::TArray<Wire::MeshPtr> > mMeshes;
	Wire::THashTable<Wire::String, Wire::Texture2DPtr> mTextures;
	Wire::THashTable<Wire::String, Wire::LightPtr> mLights;
	Wire::TArray<Wire::SpatialPtr> mStaticSpatials;
	Wire::TArray<Collider*> mColliders;

	Statistics mStatistics;
	Options mDefaultOptions;
	Options* mpOptions;
};

#endif
