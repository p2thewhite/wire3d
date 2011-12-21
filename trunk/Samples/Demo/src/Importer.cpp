#include "Importer.h"
#include "PicoPNG.h"

#include "WireEffect.h"
#include "WireIndexBuffer.h"
#include "WireLight.h"
#include "WireQuaternion.h"
#include "WireVertexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
Importer::Importer(const Char* pPath)
	:
	mpPath(pPath)
{
}

//----------------------------------------------------------------------------
Node* Importer::LoadSceneFromXml(const Char* pFilename, TArray<CameraPtr>*
	pCameras)
{
	mpCameras = pCameras;

	Int xmlSize;
	Char* pXml = Load(pFilename, xmlSize);
	if (!pXml)
	{
		return NULL;
	}

	Char* pXmlNullTerminated = WIRE_NEW Char[xmlSize+1];
	System::Memcpy(pXmlNullTerminated, xmlSize, pXml, xmlSize);
	pXmlNullTerminated[xmlSize] = 0;

	mspAlpha = WIRE_NEW StateAlpha;
	mspAlpha->BlendEnabled = true;

	rapidxml::xml_document<> doc;    // character type defaults to char
	doc.parse<0>(pXmlNullTerminated);
	Node* pRoot = WIRE_NEW Node;
	Traverse(doc.first_node(), pRoot);

	mMaterials.RemoveAll();
	mMaterialStates.RemoveAll();
	mMeshes.RemoveAll();
	mTextures.RemoveAll();

	return pRoot;
}

//----------------------------------------------------------------------------
Image2D* Importer::LoadPNG(const Char* pFilename, Bool hasMipmaps)
{
	Int pngSize;
	UChar* pPNG = reinterpret_cast<UChar*>(Load(pFilename, pngSize));
	if (!pPNG)
	{
		return NULL;
	}

	std::vector<UChar> rawImage;
	ULong width;
	ULong height;
	PicoPNG::decodePNG(rawImage, width, height, pPNG, pngSize, false);
	WIRE_DELETE[] pPNG;

	bool hasAlpha = (rawImage.size() / (height*width)) == 4;
	Image2D::FormatMode format = hasAlpha ? Image2D::FM_RGBA8888 :
		Image2D::FM_RGB888;

	size_t size = Image2D::GetBytesPerPixel(format) * width*height;
	UChar* pDst = WIRE_NEW UChar[size];
	System::Memcpy(pDst, size, &(rawImage[0]), size);

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst,
		hasMipmaps);

	return pImage;
}

//----------------------------------------------------------------------------
Char* Importer::Load(const Char* pFilename, Int& rSize)
{
	String path = String(mpPath) + String(pFilename);
	Char* pBuffer;
	Bool hasSucceeded;
	hasSucceeded = System::Load(static_cast<const Char*>(path), pBuffer,
		rSize);
	if (!hasSucceeded)
	{
		WIRE_ASSERT(false /* Could not load file */);
		return NULL;
	}

	return pBuffer;
}

//----------------------------------------------------------------------------
Float* Importer::Load32(const Char* pFilename, Int& rSize, Bool isBigEndian)
{
	Char* pBuffer = Load(pFilename, rSize);
	Float* pBuffer32 = reinterpret_cast<Float*>(pBuffer);
	if (System::IsBigEndian() != isBigEndian)
	{
		if (!pBuffer32 || (rSize & 3) != 0)
		{
			WIRE_ASSERT(false);
			return NULL;
		}

		for (Int i = 0; i < rSize; i+=4)
		{
			UChar i0 = pBuffer[i+0];
			pBuffer[i+0] = pBuffer[i+3];
			pBuffer[i+3] = i0;
			i0 = pBuffer[i+1];
			pBuffer[i+1] = pBuffer[i+2];
			pBuffer[i+2] = i0;
		}
	}

	return pBuffer32;
}

//----------------------------------------------------------------------------
void Importer::Traverse(rapidxml::xml_node<>* pXmlNode, Node* pParent)
{
	if (System::Strcmp("Leaf", pXmlNode->name()) == 0)
	{
		Geometry* pGeo = ParseLeaf(pXmlNode);
		if (pGeo)
		{
			pParent->AttachChild(pGeo);
		}

		return;
	}

	WIRE_ASSERT(System::Strcmp("Node", pXmlNode->name()) == 0);
	Node* pNode = ParseNode(pXmlNode);
	pParent->AttachChild(pNode);

	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			if (System::Strcmp("Camera", pChild->name()) == 0)
			{
				ParseCamera(pChild, pNode);
			}
			else if (System::Strcmp("Light", pChild->name()) == 0)
			{

			}
			else
			{
				Traverse(pChild, pNode);
			}
		}
	}
}

//----------------------------------------------------------------------------
Char* Importer::GetValue(rapidxml::xml_node<>* pXmlNode, const Char* pName)
{
	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (System::Strcmp(pName, attr->name()) == 0)
		{
			return attr->value();
		}
	}

	WIRE_ASSERT(false /* Xml attribute not found */);
	return NULL;
}

//----------------------------------------------------------------------------
Bool Importer::IsBigEndian(rapidxml::xml_node<>* pXmlNode)
{
	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (System::Strcmp("LittleEndian", attr->name()) == 0)
		{
			Char* pValue = attr->value();
			if (pValue)
			{
				if (*pValue == 'y')
				{
					return false;
				}
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------
void Importer::ParseCamera(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial)
{
	if (mpCameras == NULL)
	{
		return;
	}

	Spatial* pRoot = pSpatial->GetParent();
	while (pRoot && pRoot->GetParent())
	{
		pRoot = pRoot->GetParent();
	}

	Vector3F cameraLocation(0, 0, 0);
	Vector3F viewDirection(0.0F, 0.0F, 1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);

	if (pRoot)
	{
		pRoot->UpdateGS();
		cameraLocation = pSpatial->World.GetTranslate();

		Matrix34F rot = pSpatial->World.GetMatrix();
		viewDirection = rot.GetColumn(2);
		up = rot.GetColumn(1);
	}

	Char* pFov = GetValue(pXmlNode, "Fov");
	Float fov = 45;
	if (pFov)
	{
 		Int n;
 		n = sscanf(pFov, "%f", &fov);
 		WIRE_ASSERT(n == 1);
	}

	Char* pNear = GetValue(pXmlNode, "Near");
	Float near = 45;
	if (pNear)
	{
		Int n;
		n = sscanf(pNear, "%f", &near);
		WIRE_ASSERT(n == 1);
	}

	Char* pFar = GetValue(pXmlNode, "Far");
	Float far = 45;
	if (pFar)
	{
		Int n;
		n = sscanf(pFar, "%f", &far);
		WIRE_ASSERT(n == 1);
	}
	
	Vector3F right = viewDirection.Cross(up);
	Camera* pCam = WIRE_NEW Camera;
	pCam->SetFrame(cameraLocation, viewDirection, up, right);
	pCam->SetFrustum(fov, 640.0f / 480.0f , near, far);
	mpCameras->Append(pCam);
}

//----------------------------------------------------------------------------
void Importer::ParseTransformation(rapidxml::xml_node<>* pXmlNode,
	Spatial* pSpatial)
{
	Vector3F t;
	QuaternionF r;
	Vector3F s;

	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (System::Strcmp("Pos", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f", &t.X(), &t.Y(), &t.Z());
			WIRE_ASSERT(n == 3);
		}
		else if (System::Strcmp("Rot", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f, %f", &r.W(), &r.X(),
				&r.Y(), &r.Z());
			WIRE_ASSERT(n == 4);
		}
		else if (System::Strcmp("Scale", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f", &s.X(), &s.Y(), &s.Z());
			WIRE_ASSERT(n == 3);
		}
	}

	if ((t.X() != 0) || (t.Y() != 0) || (t.Z() != 0))
	{
		pSpatial->Local.SetTranslate(t);
	}

	if ((r.W() != 1) || (r.X() != 0) || (r.Y() != 0) || (r.Z() != 0))
	{
		Matrix3F m;
		r.ToRotationMatrix(m);
		pSpatial->Local.SetRotate(m);
	}

	if ((s.X() != 1.0f) || (s.Y() != 1.0f) || (s.Z() != 1.0f))
	{
		if ((s.X() == s.Y()) && (s.X() == s.Z()))
		{
			pSpatial->Local.SetUniformScale(s.X());
		}
		else
		{
			pSpatial->Local.SetScale(s);
		}
	}
}

//----------------------------------------------------------------------------
Node* Importer::ParseNode(rapidxml::xml_node<>* pXmlNode)
{
	Node* pNode = WIRE_NEW Node;

	Char* pName = GetValue(pXmlNode, "Name");
	if (pName)
	{
		pNode->SetName(pName);
	}

	ParseTransformation(pXmlNode, pNode);

	return pNode;
}

//----------------------------------------------------------------------------
Geometry* Importer::ParseLeaf(rapidxml::xml_node<>* pXmlNode)
{
	Mesh* pMesh = NULL;
	Material* pMaterial = NULL;

	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			if (System::Strcmp("Mesh", pChild->name()) == 0)
			{
				pMesh = ParseMesh(pChild);
			}
			else if (System::Strcmp("Material", pChild->name()) == 0)
			{
				pMaterial = ParseMaterial(pChild);
			}
		}
	}

	if (!pMesh)
	{
		WIRE_ASSERT(false /* Mesh is missing */);
		return NULL;
	}

	if (pMesh->GetVertexBuffer()->GetAttributes().HasColor())
	{
		if (pMaterial && pMaterial->GetTextureQuantity() > 0)
		{
			pMaterial->SetBlendMode(Material::BM_MODULATE);
		}
	}

	Geometry* pGeo = WIRE_NEW Geometry(pMesh, pMaterial);
	Char* pName = GetValue(pXmlNode, "Name");
	if (pName)
	{
		pGeo->SetName(pName);
	}

	Bool hasAlpha = false;
	if (pMaterial)
	{
		for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
		{
			Image2D* pImage2D = pMaterial->GetTexture(i)->GetImage();
			if (pImage2D->GetFormat() == Image2D::FM_RGBA4444 ||
				pImage2D->GetFormat() == Image2D::FM_RGBA8888)
			{
				hasAlpha = true;
			}
		}

		StateMaterial** ppState = mMaterialStates.Find(pMaterial);
		if (ppState && *ppState)
		{
			pGeo->AttachState(*ppState);
		}
	}

	if (hasAlpha)
	{
//		pGeo->AttachState(mspAlpha);
	}

	ParseTransformation(pXmlNode, pGeo);

	return pGeo;
}

//----------------------------------------------------------------------------
Mesh* Importer::ParseMesh(rapidxml::xml_node<>* pXmlNode)
{
	Char* pName = GetValue(pXmlNode, "Name");
	if (!pName)
	{
		WIRE_ASSERT(false /* Mesh has no name */);
		return NULL;
	}

	Mesh** pValue = mMeshes.Find(pName);
	if (pValue)
	{
		WIRE_ASSERT(*pValue);
		return *pValue;
	}

	Char* pVerticesName = NULL;
	Char* pIndicesName = NULL;
	Char* pNormalsName = NULL;
	Char* pColorsName = NULL;
	TArray<Char*> uvSetNames(8,8);
	Bool vBigEndian = true;
	Bool iBigEndian = true;
	Bool nBigEndian = true;
	Bool cBigEndian = true;
	TArray<Bool> uvBigEndian(8,8);

	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		if (!pVerticesName &&
			System::Strcmp("Vertices", pChild->name()) == 0)
		{
			pVerticesName = GetValue(pChild, "Name");
			vBigEndian = IsBigEndian(pChild);
		}
		else if (!pIndicesName &&
			System::Strcmp("Indices", pChild->name()) == 0)
		{
			pIndicesName = GetValue(pChild, "Name");
			iBigEndian = IsBigEndian(pChild);
		}
		else if (!pNormalsName &&
			System::Strcmp("Normals", pChild->name()) == 0)
		{
			pNormalsName = GetValue(pChild, "Name");
			nBigEndian = IsBigEndian(pChild);
		}
		else if (!pColorsName &&
			System::Strcmp("Colors", pChild->name()) == 0)
		{
			pColorsName = GetValue(pChild, "Name");
			cBigEndian = IsBigEndian(pChild);
		}
		else if (System::Strncmp("Uv", pChild->name(), 2) == 0)
		{
			UInt nr;
			Int n;
			n = sscanf(pChild->name(), "Uv%d", &nr);
			WIRE_ASSERT(n == 1);
			uvSetNames.Append(GetValue(pChild, "Name"));
			uvBigEndian.Append(IsBigEndian(pChild));
		}
	}

	if (!pVerticesName || !pIndicesName)
	{
		WIRE_ASSERT(false /* Mesh has no vertices or indices */);
		return NULL;
	}

	VertexAttributes va;
	va.SetPositionChannels(3);
	Int verticesSize;
	Float* pVertices = Load32(pVerticesName, verticesSize, vBigEndian);

	Int normalsSize;
	Float* pNormals = NULL;
	if (pNormalsName)
	{
		va.SetNormalChannels(3);
		pNormals = Load32(pNormalsName, normalsSize, iBigEndian);
		if (verticesSize != normalsSize)
		{
			WIRE_ASSERT(false /* vertices and normals do not match */);
			return NULL;
		}
	}

	Int colorsSize;
	Float* pColors = NULL;
	if (pColorsName)
	{
		va.SetColorChannels(4);
		pColors = Load32(pColorsName, colorsSize, cBigEndian);
		if (verticesSize/(3*sizeof(Float)) != colorsSize/(4*sizeof(Float)))
		{
			WIRE_ASSERT(false /* vertices and colors do not match */);
			return NULL;
		}
	}

	TArray<Float*> uvSets(uvSetNames.GetQuantity());
	TArray<Int> uvSetSizes(uvSetNames.GetQuantity());
	for (UInt i = 0; i < uvSetNames.GetQuantity(); i++)
	{
		uvSetSizes.Append(0);
		uvSets.Append(Load32(uvSetNames[i], uvSetSizes[i], uvBigEndian[i]));
		va.SetTCoordChannels(2, i);
		if (verticesSize/(3*sizeof(Float)) != uvSetSizes[i]/(2*sizeof(Float)))
		{
			WIRE_ASSERT(false /* vertices and uv sets do not match */);
			return NULL;
		}	
	}

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(va, verticesSize/
		(3*sizeof(Float)));

	for (UInt i = 0; i < (verticesSize/(3*sizeof(Float))); i++)
	{
		if (pVertices)
		{
			Vector3F v;
			v.X() = *pVertices++;
			v.Y() = *pVertices++;
			v.Z() = *pVertices++;
			pVertexBuffer->Position3(i) = v;
		}

		if (pNormals)
		{
			Vector3F n;
			n.X() = *pNormals++;
			n.Y() = *pNormals++;
			n.Z() = *pNormals++;
			pVertexBuffer->Normal3(i) = n;
		}

		if (pColors)
		{
			ColorRGBA c;
			c.R() = *pColors++;
			c.G() = *pColors++;
			c.B() = *pColors++;
			c.A() = *pColors++;
			pVertexBuffer->Color4(i) = c;
		}

		for (UInt j = 0; j < uvSets.GetQuantity(); j++)
		{
			Vector2F v(uvSets[j][i*2], uvSets[j][i*2+1]);
			pVertexBuffer->TCoord2(i, j) = v;
		}
	}

	Int indicesSize;
	UInt* pIndices = reinterpret_cast<UInt*>(Load32(pIndicesName,
		indicesSize, iBigEndian));
	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(pIndices,
		indicesSize/sizeof(UInt));

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	mMeshes.Insert(pName, pMesh);
	return pMesh;
}

//----------------------------------------------------------------------------
Material* Importer::ParseMaterial(rapidxml::xml_node<>* pXmlNode)
{
	Char* pName = GetValue(pXmlNode, "Name");
	if (!pName)
	{
		WIRE_ASSERT(false /* Material has no name */);
		return NULL;
	}

	Material** pValue = mMaterials.Find(pName);
	if (pValue)
	{
		WIRE_ASSERT(*pValue);
		return *pValue;
	}

	Material* pMaterial = WIRE_NEW Material;
	StateMaterial* pMaterialState = NULL;

	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			if (System::Strcmp("Texture", pChild->name()) == 0)
			{
				Texture2D* pTex = ParseTexture(pChild);
				Material::BlendMode mode = pMaterial->GetTextureQuantity() >
					0 ? Material::BM_MODULATE : Material::BM_REPLACE;
				pMaterial->AddTexture(pTex, mode);
			}
			else if (System::Strcmp("MaterialState", pChild->name()) == 0)
			{
				Char* pCol = GetValue(pChild, "Ambient");
				if (pCol)
				{
					Int n;
					ColorRGBA c;
					n = sscanf(pCol, "%f, %f, %f, %f", &c.R(), &c.G(), &c.B(),&c.A());

					WIRE_ASSERT(n == 4);
					if (pMaterialState)
					{
						// multiple MaterialStates defined within one Material
						// we only use the last encountered.
						WIRE_ASSERT(false);
						WIRE_DELETE pMaterialState;
					}

					pMaterialState = WIRE_NEW StateMaterial;
					pMaterialState->Ambient = c;
				}
			}
		}
	}

	if (pMaterial->GetTextureQuantity() == 0)
	{
		WIRE_DELETE pMaterial;
		return NULL;
	}

	mMaterials.Insert(pName, pMaterial);

	if (pMaterialState)
	{
		mMaterialStates.Insert(pMaterial, pMaterialState);
	}

	return pMaterial;
}

//----------------------------------------------------------------------------
Texture2D* Importer::ParseTexture(rapidxml::xml_node<>* pXmlNode)
{
	Char* pName = GetValue(pXmlNode, "Name");
	if (!pName)
	{
		WIRE_ASSERT(false /* No Texture filename found */);
		return NULL;
	}

	Texture2D** pValue = mTextures.Find(pName);
	if (pValue)
	{
		WIRE_ASSERT(*pValue);
		return *pValue;
	}

	UInt mipmapCount = 1;
	Texture2D::FilterType filter = Texture2D::FT_LINEAR_LINEAR;
	Texture2D::WrapType warp = Texture2D::WT_CLAMP;
	UInt anisoLevel = 0;

	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (System::Strcmp("Mipmaps", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%d", &mipmapCount);
			WIRE_ASSERT(n == 1);	
		}
		else if (System::Strcmp("FilterMode", attr->name()) == 0)
		{
			if (System::Strcmp("Point", attr->value()) == 0)
			{
				filter = Texture2D::FT_NEAREST_NEAREST;
			}
			else if (System::Strcmp("Bilinear", attr->value()) == 0)
			{
				filter = Texture2D::FT_LINEAR_NEAREST;
			}
			else if (System::Strcmp("Trilinear", attr->value()) == 0)
			{
				filter = Texture2D::FT_LINEAR_LINEAR;
			}
		}
		else if (System::Strcmp("AnisoLevel", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%d", &anisoLevel);
			WIRE_ASSERT(n == 1);	
		}
		else if (System::Strcmp("WrapMode", attr->name()) == 0)
		{
			if (System::Strcmp("Repeat", attr->value()) == 0)
			{
				warp = Texture2D::WT_REPEAT;
			}
			else if (System::Strcmp("Clamp", attr->value()) == 0)
			{
				warp = Texture2D::WT_CLAMP;
			}
		}
	}

	Image2D* pImage = LoadPNG(pName, (mipmapCount > 1));
	if (!pImage)
	{
		return NULL;
	}

	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetFilterType(filter);
	pTexture->SetWrapType(0, warp);
	pTexture->SetWrapType(1, warp);
	pTexture->SetAnisotropyValue(static_cast<Float>(anisoLevel));

	mTextures.Insert(pName, pTexture);

	return pTexture;
}
