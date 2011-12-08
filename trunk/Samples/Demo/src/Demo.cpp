// Sample1 - Basic Rendering
// This sample demonstrates how to create a transparent, textured cube and
// render it using different render states and a light.

#include "Demo.h"
#include "PicoPNG.h"

using namespace Wire;

WIRE_APPLICATION(Demo);

//----------------------------------------------------------------------------
Bool Demo::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mpPath = "Data/"; // TODO: init "" when not used
	mspRoot = LoadScene("InnsbruckMobile.xml");

	// Once we finished creating the scene graph, we update the graph's
	// render states. This propagates the fog state to all child nodes.
	// Whenever you change the graph's render state (using Attach-/
	// DetachState() on any of its children), you must call UpdateRS().
	mspRoot->UpdateRS();

	// setup our camera at the origin, looking down the -z axis with y up
	Vector3F cameraLocation(0.0F, 0.0F, 0.0F);
	Vector3F viewDirection(1.0F, 0.0F, 0.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// specify FOV, aspect ratio, near and far plane of the frustum
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(60, width / height , 0.1F, 30000.0F);

	// the culler needs to know which camera to use when performing its task
	mCuller.SetCamera(mspCamera);

	mAngle = 0.0F;
	mLastTime = System::GetTime();
	return true;
}

//----------------------------------------------------------------------------
void Demo::OnIdle()
{
	// This function is called by the framework's render loop until the
	// application exits. Perform all your rendering here.

	// Determine how much time has passed since the last call, so we can
	// move our objects independently of the actual frame rate.
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Node* Demo::LoadScene(const Char* pFilename)
{
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
	Node* pNode = DynamicCast<Node>(Traverse(doc.first_node()));

	mMaterials.RemoveAll();
	mMeshes.RemoveAll();
	mTextures.RemoveAll();

	return pNode;
}

//----------------------------------------------------------------------------
Image2D* Demo::LoadImage(const Char* pFilename, Bool hasMipmaps)
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
	System::Memcpy(pDst, size, rawImage.data(), size);

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst,
		hasMipmaps);

	return pImage;
}

//----------------------------------------------------------------------------
Char* Demo::Load(const Char* pFilename, Int& rSize)
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
Spatial* Demo::Traverse(rapidxml::xml_node<>* pXmlNode)
{
	if (System::Strcmp("Leaf", pXmlNode->name()) == 0)
	{
		return ParseLeaf(pXmlNode);	
	}

	WIRE_ASSERT(System::Strcmp("Node", pXmlNode->name()) == 0);
	Node* pNode = ParseNode(pXmlNode);

	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			Spatial* pSpatial = Traverse(pChild);
			WIRE_ASSERT(pSpatial);
			pNode->AttachChild(pSpatial);
		}
	}

	return pNode;
}

//----------------------------------------------------------------------------
Char* Demo::GetValue(rapidxml::xml_node<>* pXmlNode, const Char* pName)
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
void Demo::ParseTransformation(rapidxml::xml_node<>* pXmlNode,
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
Node* Demo::ParseNode(rapidxml::xml_node<>* pXmlNode)
{
	Node* pNode = WIRE_NEW Node;

	ParseTransformation(pXmlNode, pNode);

	return pNode;
}

//----------------------------------------------------------------------------
Geometry* Demo::ParseLeaf(rapidxml::xml_node<>* pXmlNode)
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
	}

	Geometry* pGeo = WIRE_NEW Geometry(pMesh, pMaterial);
	if (hasAlpha)
	{
		pGeo->AttachState(mspAlpha);
	}

	ParseTransformation(pXmlNode, pGeo);

	return pGeo;
}

//----------------------------------------------------------------------------
Mesh* Demo::ParseMesh(rapidxml::xml_node<>* pXmlNode)
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

	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		if (!pVerticesName &&
			System::Strcmp("Vertices", pChild->name()) == 0)
		{
			pVerticesName = GetValue(pChild, "Name");
		}
		else if (!pIndicesName &&
			System::Strcmp("Indices", pChild->name()) == 0)
		{
			pIndicesName = GetValue(pChild, "Name");
		}
		else if (!pNormalsName &&
			System::Strcmp("Normals", pChild->name()) == 0)
		{
			pNormalsName = GetValue(pChild, "Name");
		}
		else if (!pColorsName &&
			System::Strcmp("Colors", pChild->name()) == 0)
		{
			pColorsName = GetValue(pChild, "Name");
		}
		else if (System::Strncmp("Uv", pChild->name(), 2) == 0)
		{
			UInt nr;
			Int n;
			n = sscanf(pChild->name(), "Uv%d", &nr);
			WIRE_ASSERT(n == 1);
			uvSetNames.Append(GetValue(pChild, "Name"));
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
	Float* pVertices = reinterpret_cast<Float*>(Load(pVerticesName,
		verticesSize));

	Int normalsSize;
	Float* pNormals = NULL;
	if (pNormalsName)
	{
		va.SetNormalChannels(3);
		pNormals = reinterpret_cast<Float*>(Load(pNormalsName, normalsSize));
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
		pColors = reinterpret_cast<Float*>(Load(pColorsName, colorsSize));
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
		uvSets.Append(reinterpret_cast<Float*>(Load(uvSetNames[i],
			uvSetSizes[i])));
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
			Vector3F v(*pVertices++, *pVertices++, *pVertices++);
			pVertexBuffer->Position3(i) = v;
		}

		if (pNormals)
		{
			Vector3F v(*pNormals++, *pNormals++, *pNormals++);
			pVertexBuffer->Normal3(i) = v;
		}

		if (pColors)
		{
			ColorRGBA c(*pColors++, *pColors++, *pColors++, *pColors++);
			pVertexBuffer->Color4(i) = c;
		}

		for (UInt j = 0; j < uvSets.GetQuantity(); j++)
		{
			Vector2F v(uvSets[j][i*2], uvSets[j][i*2+1]);
			pVertexBuffer->TCoord2(i, j) = v;
		}
	}

	Int indicesSize;
	UInt* pIndices = reinterpret_cast<UInt*>(Load(pIndicesName, indicesSize));
	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(pIndices,
		indicesSize/sizeof(UInt));

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	mMeshes.Insert(pName, pMesh);
	return pMesh;
}

//----------------------------------------------------------------------------
Material* Demo::ParseMaterial(rapidxml::xml_node<>* pXmlNode)
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
		}
	}

	if (pMaterial->GetTextureQuantity() == 0)
	{
		WIRE_DELETE pMaterial;
		return NULL;
	}

	mMaterials.Insert(pName, pMaterial);
	return pMaterial;
}

//----------------------------------------------------------------------------
Texture2D* Demo::ParseTexture(rapidxml::xml_node<>* pXmlNode)
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

	Image2D* pImage = LoadImage(pName, (mipmapCount > 1));
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
