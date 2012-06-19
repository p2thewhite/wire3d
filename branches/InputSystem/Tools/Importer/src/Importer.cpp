#include "Importer.h"
#include "PicoPNG.h"

#include "WireEffect.h"
#include "WireIndexBuffer.h"
#include "WireLight.h"
#include "WireQuaternion.h"
#include "WireVertexBuffer.h"
#include "WireStateAlpha.h"
#include "WireStateCull.h"
#include "WireStateFog.h"
#include "WireStateWireframe.h"
#include "WireStateZBuffer.h"
#include "WireTStack.h"

using namespace Wire;

//----------------------------------------------------------------------------
Importer::Importer(const Char* pPath,
	Bool materialsWithEqualNamesAreIdentical)
	:
	mpPath(pPath),
	mMaterialsWithEqualNamesAreIdentical(materialsWithEqualNamesAreIdentical)
{
}

//----------------------------------------------------------------------------
Node* Importer::LoadSceneFromXml(const Char* pFilename, TArray<CameraPtr>*
	pCameras)
{
	ResetStatistics();
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
	WIRE_DELETE[] pXml;

	rapidxml::xml_document<> doc;    // character type defaults to char
	doc.parse<0>(pXmlNullTerminated);
	Node* pRoot = WIRE_NEW Node;
	pRoot->SetName(pFilename);
	Traverse(doc.first_node(), pRoot);

	WIRE_DELETE[] pXmlNullTerminated;

	mMaterials.RemoveAll();
	mMaterialStates.RemoveAll();
	mMeshes.RemoveAll();
	mTextures.RemoveAll();

	pRoot->UpdateRS();
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

	Image2D* pImage2D = DecodePNG(pPNG, pngSize, hasMipmaps);
	WIRE_DELETE[] pPNG;

	return pImage2D;
}

//----------------------------------------------------------------------------
Image2D* Importer::DecodePNG(const UChar* pPNG, size_t pngSize,
	Bool hasMipmaps)
{
	std::vector<UChar> rawImage;
	ULong width;
	ULong height;
	PicoPNG::decodePNG(rawImage, width, height, pPNG, pngSize, false);

	Bool hasAlpha = (rawImage.size() / (height*width)) == 4;
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
void Importer::Free32(Float* pFloats)
{
	if (!pFloats)
	{
		return;
	}

	UChar* pUChar = reinterpret_cast<UChar*>(pFloats);
	WIRE_DELETE[] pUChar;
}

//----------------------------------------------------------------------------
const Importer::Statistics* Importer::GetStatistics()
{
	return &mStatistics;
}

//----------------------------------------------------------------------------
void Importer::ResetStatistics()
{
	mStatistics.GeometryCount = 0;
	mStatistics.NodeCount = 0;
	mStatistics.TextureCount = 0;
	mStatistics.MaterialCount = 0;
	mStatistics.VertexBufferCount = 0;
	mStatistics.IndexBufferCount = 0;
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
			ParseComponents(pChild, pNode);

			if (System::Strcmp("Node", pChild->name()) == 0 ||
				System::Strcmp("Leaf", pChild->name()) == 0)
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
Buffer::UsageType Importer::GetUsageType(rapidxml::xml_node<>* pXmlNode)
{
	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (System::Strcmp("Usage", attr->name()) == 0)
		{
			Char* pValue = attr->value();
			if (pValue)
			{
				if (System::Strcmp("STATIC", pValue) == 0)
				{
					return Buffer::UT_STATIC;
				}
				else if (System::Strcmp("DYNAMIC", pValue) == 0)
				{
					return Buffer::UT_DYNAMIC;
				}
				else if (System::Strcmp("DYNAMIC_WRITE_ONLY", pValue) == 0)
				{
					return  Buffer::UT_DYNAMIC_WRITE_ONLY;
				}
			}
		}
	}

	return Buffer::UT_STATIC;
}

//----------------------------------------------------------------------------
Float Importer::GetFloat(rapidxml::xml_node<>* pXmlNode, const Char* pName)
{
	Char* pFloat = GetValue(pXmlNode, pName);
	Float f = 0;
	if (pFloat)
	{
		Int n;
		n = sscanf(pFloat, "%f", &f);
		WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
	}

	return f;
}

//----------------------------------------------------------------------------
Bool Importer::GetBool(rapidxml::xml_node<>* pXmlNode, const Char* pName)
{
	Char* pBool = GetValue(pXmlNode, pName);
	UInt b = 0;
	if (pBool)
	{
		Int n;
		n = sscanf(pBool, "%d", &b);
		WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
	}

	return (b != 0);
}

//----------------------------------------------------------------------------
ColorRGB Importer::GetColorRGB(rapidxml::xml_node<>* pXmlNode, const Char*
	pName)
{
	ColorRGB c;
	Char* pCol = GetValue(pXmlNode, pName);
	if (pCol)
	{
		Int n;
		n = sscanf(pCol, "%f, %f, %f", &c.R(), &c.G(), &c.B());
		WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
	}

	return c;
}

//----------------------------------------------------------------------------
void Importer::UpdateGS(Spatial* pSpatial)
{
	Spatial* pRoot = pSpatial->GetParent();
	while (pRoot && pRoot->GetParent())
	{
		pRoot = pRoot->GetParent();
	}

	if (pRoot)
	{
		pRoot->UpdateGS();
	}
	else
	{
		pSpatial->UpdateGS();
	}
}

//----------------------------------------------------------------------------
void Importer::ParseLight(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial)
{
	UpdateGS(pSpatial);

	Char* pType = GetValue(pXmlNode, "Type");
	Light* pLight = WIRE_NEW Light;
	Light::LightType lt = Light::LT_POINT;

	if (System::Strcmp("Point", pType) == 0)
	{
		lt = Light::LT_POINT;
	}
	else if (System::Strcmp("Directional", pType) == 0)
	{
		lt = Light::LT_DIRECTIONAL;
	}
	else if (System::Strcmp("Spot", pType) == 0)
	{
		lt = Light::LT_SPOT;
//		Float angleDeg = GetFloat()
	}

	pLight->Type = lt;
	pLight->Position = pSpatial->World.GetTranslate();
	pLight->Ambient = GetColorRGB(pXmlNode, "Ambient");
	pLight->Color = GetColorRGB(pXmlNode, "Color");

	pSpatial->AttachLight(pLight);
}

//----------------------------------------------------------------------------
void Importer::ParseCamera(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial)
{
	if (mpCameras == NULL)
	{
		return;
	}

	Vector3F cameraLocation(0, 0, 0);
	Vector3F viewDirection(0.0F, 0.0F, 1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);

	UpdateGS(pSpatial);
	cameraLocation = pSpatial->World.GetTranslate();
	Matrix34F rot = pSpatial->World.GetMatrix();
	viewDirection = rot.GetColumn(2);
	up = rot.GetColumn(1);
	Vector3F right = viewDirection.Cross(up);

	Float fov = GetFloat(pXmlNode, "Fov");
	Float near = GetFloat(pXmlNode, "Near");
	Float far = GetFloat(pXmlNode, "Far");
	Camera* pCam = WIRE_NEW Camera(fov != 0.0F);
	if (fov != 0.0F)
	{
		pCam->SetFrame(cameraLocation, viewDirection, up, right);
	}

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
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
		}
		else if (System::Strcmp("Rot", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f, %f", &r.W(), &r.X(),
				&r.Y(), &r.Z());
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 4);
		}
		else if (System::Strcmp("Scale", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f", &s.X(), &s.Y(), &s.Z());
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
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
	mStatistics.NodeCount++;

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
	mStatistics.GeometryCount++;
	Char* pName = GetValue(pXmlNode, "Name");
	if (pName)
	{
		pGeo->SetName(pName);
	}

	if (pMaterial)
	{
		TArray<State*>* pStateList = mMaterialStates.Find(pMaterial);
		if (pStateList)
		{
			for (UInt i = 0; i < pStateList->GetQuantity(); i++)
			{
				pGeo->AttachState((*pStateList)[i]);
			}
		}
	}

	ParseTransformation(pXmlNode, pGeo);

	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			ParseComponents(pChild, pGeo);
		}
	}

	return pGeo;
}

//----------------------------------------------------------------------------
void Importer::ParseComponents(rapidxml::xml_node<>* pXmlNode, Spatial*
	pSpatial)
{
	State* pState = ParseRenderStates(pXmlNode);
	if (pState)
	{
		pSpatial->AttachState(pState);
	}
	else if (System::Strcmp("Camera", pXmlNode->name()) == 0)
	{
		ParseCamera(pXmlNode, pSpatial);
	}
	else if (System::Strcmp("Light", pXmlNode->name()) == 0)
	{
		ParseLight(pXmlNode, pSpatial);
	}
}

//----------------------------------------------------------------------------
State* Importer::ParseRenderStates(rapidxml::xml_node<>* pXmlNode)
{
	if (System::Strcmp("MaterialState", pXmlNode->name()) == 0)
	{
		StateMaterial* pMaterialState = WIRE_NEW StateMaterial;

		Char* pCol = GetValue(pXmlNode, "Ambient");
		if (pCol)
		{
			Int n;
			ColorRGBA c;
			n = sscanf(pCol, "%f, %f, %f, %f", &c.R(), &c.G(), &c.B(),&c.A());
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 4);

			pMaterialState->Ambient = c;
		}

		return pMaterialState;
	}
	else if (System::Strcmp("AlphaState", pXmlNode->name()) == 0)
	{
		StateAlpha* pAlphaState = WIRE_NEW StateAlpha;
		pAlphaState->BlendEnabled = GetBool(pXmlNode, "Enabled");

		Char* pSrcBlend = GetValue(pXmlNode, "SrcBlendMode");
		if (pSrcBlend)
		{
			if (System::Strcmp("ZERO", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ZERO;
			}
			else if (System::Strcmp("ONE", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE;
			}
			else if (System::Strcmp("DST_COLOR", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_DST_COLOR;
			}
			else if (System::Strcmp("ONE_MINUS_DST_COLOR", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE_MINUS_DST_COLOR;
			}
			else if (System::Strcmp("SRC_ALPHA", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_SRC_ALPHA;
			}
			else if (System::Strcmp("ONE_MINUS_SRC_ALPHA", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE_MINUS_SRC_ALPHA;
			}
			else if (System::Strcmp("DST_ALPHA", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_DST_ALPHA;
			}
			else if (System::Strcmp("ONE_MINUS_DST_ALPHA", pSrcBlend) == 0)
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE_MINUS_DST_ALPHA;
			}
		}

		Char* pDstBlend = GetValue(pXmlNode, "DstBlendMode");
		if (pDstBlend)
		{
			if (System::Strcmp("ZERO", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ZERO;
			}
			else if (System::Strcmp("ONE", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE;
			}
			else if (System::Strcmp("SRC_COLOR", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_SRC_COLOR;
			}
			else if (System::Strcmp("ONE_MINUS_SRC_COLOR", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE_MINUS_SRC_COLOR;
			}
			else if (System::Strcmp("SRC_ALPHA", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_SRC_ALPHA;
			}
			else if (System::Strcmp("ONE_MINUS_SRC_ALPHA", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE_MINUS_SRC_ALPHA;
			}
			else if (System::Strcmp("DST_ALPHA", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_DST_ALPHA;
			}
			else if (System::Strcmp("ONE_MINUS_DST_ALPHA", pDstBlend) == 0)
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE_MINUS_DST_ALPHA;
			}
		}

		return pAlphaState;
	}
	else if (System::Strcmp("ZBufferState", pXmlNode->name()) == 0)
	{
		StateZBuffer* pZBufferState = WIRE_NEW StateZBuffer;
		pZBufferState->Enabled = GetBool(pXmlNode, "Enabled");
		pZBufferState->Writable = GetBool(pXmlNode, "Writable");

		Char* pCmpFunc = GetValue(pXmlNode, "CmpFunc");
		if (pCmpFunc)
		{
			if (System::Strcmp("NEVER", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_NEVER;
			}
			else if (System::Strcmp("LESS", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_LESS;
			}
			else if (System::Strcmp("EQUAL", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_EQUAL;
			}
			else if (System::Strcmp("LEQUAL", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_LEQUAL;
			}
			else if (System::Strcmp("GREATER", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_GREATER;
			}
			else if (System::Strcmp("NOTEQUAL", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_NOTEQUAL;
			}
			else if (System::Strcmp("GEQUAL", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_GEQUAL;
			}
			else if (System::Strcmp("ALWAYS", pCmpFunc) == 0)
			{
				pZBufferState->Compare = StateZBuffer::CF_ALWAYS;
			}
		}

		return pZBufferState;
	}
	else if (System::Strcmp("CullState", pXmlNode->name()) == 0)
	{
		StateCull* pCullState = WIRE_NEW StateCull;
		pCullState->Enabled = GetBool(pXmlNode, "Enabled");

		Char* pMode = GetValue(pXmlNode, "Mode");
		if (pMode)
		{
			if (System::Strcmp("OFF", pMode) == 0)
			{
				pCullState->CullFace = StateCull::CM_OFF;
			}
			else if (System::Strcmp("BACK", pMode) == 0)
			{
				pCullState->CullFace = StateCull::CM_BACK;
			}
			else if (System::Strcmp("FRONT", pMode) == 0)
			{
				pCullState->CullFace = StateCull::CM_FRONT;
			}
		}

		return pCullState;
	}
	else if (System::Strcmp("WireframeState", pXmlNode->name()) == 0)
	{
		StateWireframe* pWireframeState = WIRE_NEW StateWireframe;
		pWireframeState->Enabled = GetBool(pXmlNode, "Enabled");

		return pWireframeState;
	}
	else if (System::Strcmp("FogState", pXmlNode->name()) == 0)
	{
		StateFog* pFogState = WIRE_NEW StateFog;
		pFogState->Enabled = GetBool(pXmlNode, "Enabled");
		pFogState->Color = GetColorRGB(pXmlNode, "Color");
		pFogState->Start = GetFloat(pXmlNode, "Start");
		pFogState->End = GetFloat(pXmlNode, "End");

		Char* pFunc = GetValue(pXmlNode, "Func");
		if (pFunc)
		{
			if (System::Strcmp("LINEAR", pFunc) == 0)
			{
				pFogState->DensityFunc = StateFog::DF_LINEAR;
			}
			else if (System::Strcmp("EXP", pFunc) == 0)
			{
				pFogState->DensityFunc = StateFog::DF_EXP;
			}
			else if (System::Strcmp("EXPSQR", pFunc) == 0)
			{
				pFogState->DensityFunc = StateFog::DF_EXPSQR;
			}
		}

		return pFogState;
	}

	return NULL;
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
	Buffer::UsageType vUsage = Buffer::UT_STATIC;
	Buffer::UsageType iUsage = Buffer::UT_STATIC;

	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		if (!pVerticesName &&
			System::Strcmp("Vertices", pChild->name()) == 0)
		{
			pVerticesName = GetValue(pChild, "Name");
			vBigEndian = IsBigEndian(pChild);
			vUsage = GetUsageType(pChild);
		}
		else if (!pIndicesName &&
			System::Strcmp("Indices", pChild->name()) == 0)
		{
			pIndicesName = GetValue(pChild, "Name");
			iBigEndian = IsBigEndian(pChild);
			iUsage = GetUsageType(pChild);
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
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
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
		pNormals = Load32(pNormalsName, normalsSize, nBigEndian);
		if (verticesSize != normalsSize)
		{
			WIRE_ASSERT(false /* vertices and normals do not match */);
			Free32(pVertices);
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
			Free32(pNormals);
			Free32(pVertices);
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
			Free32(pColors);
			Free32(pNormals);
			Free32(pVertices);
			return NULL;
		}	
	}

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(va, verticesSize/
		(3*sizeof(Float)), vUsage);
	mStatistics.VertexBufferCount++;

	Float* pTempVertices = pVertices;
	Float* pTempNormals = pNormals;
	Float* pTempColors = pColors;
	for (UInt i = 0; i < (verticesSize/(3*sizeof(Float))); i++)
	{
		if (pVertices)
		{
			Vector3F v;
			v.X() = *pTempVertices++;
			v.Y() = *pTempVertices++;
			v.Z() = *pTempVertices++;
			pVertexBuffer->Position3(i) = v;
		}

		if (pNormals)
		{
			Vector3F n;
			n.X() = *pTempNormals++;
			n.Y() = *pTempNormals++;
			n.Z() = *pTempNormals++;
			pVertexBuffer->Normal3(i) = n;
		}

		if (pColors)
		{
			ColorRGBA c;
			c.R() = *pTempColors++;
			c.G() = *pTempColors++;
			c.B() = *pTempColors++;
			c.A() = *pTempColors++;
			pVertexBuffer->Color4(i) = c;
		}

		for (UInt j = 0; j < uvSets.GetQuantity(); j++)
		{
			Vector2F v(uvSets[j][i*2], uvSets[j][i*2+1]);
			pVertexBuffer->TCoord2(i, j) = v;
		}
	}

	for (UInt i = 0; i < uvSets.GetQuantity(); i++)
	{
		Free32(uvSets[i]);
	}

	Free32(pColors);
	Free32(pNormals);
	Free32(pVertices);

	Int indicesSize;
	UInt* pIndices = reinterpret_cast<UInt*>(Load32(pIndicesName,
		indicesSize, iBigEndian));
	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(
		indicesSize/sizeof(UInt), iUsage);
	mStatistics.IndexBufferCount++;
	for (UInt i = 0; i < indicesSize/sizeof(UInt); i++)
	{
		WIRE_ASSERT(pIndices[i] < 65536);
		(*pIndexBuffer)[i] = static_cast<UShort>(pIndices[i]);
	}

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

	if (mMaterialsWithEqualNamesAreIdentical)
	{
		Material** pValue = mMaterials.Find(pName);
		if (pValue)
		{
			WIRE_ASSERT(*pValue);
			return *pValue;
		}
	}

	Material* pMaterial = WIRE_NEW Material;
	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			if (System::Strcmp("Texture", pChild->name()) == 0)
			{
				Material::BlendMode bm = Material::BM_QUANTITY;
				Texture2D* pTex = ParseTexture(pChild, bm);

				if (bm == Material::BM_QUANTITY)
				{
					bm = pMaterial->GetTextureQuantity() > 0 ?
						Material::BM_MODULATE : Material::BM_REPLACE;
				}

				pMaterial->AddTexture(pTex, bm);
			}
			else
			{
				State* pState = ParseRenderStates(pChild);
				if (pState)
				{
					TArray<State*>* pStateList = mMaterialStates.Find(pMaterial);
					if (pStateList)
					{
						for (UInt i = 0; i < pStateList->GetQuantity(); i++)
						{
							if ((*pStateList)[i]->GetStateType() == pState->
								GetStateType())
							{
								WIRE_ASSERT(false /* multiple States of the same type encountered */);
							}
						}

						pStateList->Append(pState);
					}
					else
					{
						TArray<State*> stateList;
						stateList.Append(pState);
						mMaterialStates.Insert(pMaterial, stateList);
					}
				}
			}
		}
	}

	if (pMaterial->GetTextureQuantity() == 0)
	{
		TArray<State*>* pStateList = mMaterialStates.Find(pMaterial);
		if (pStateList)
		{
			for (UInt i = 0; i < pStateList->GetQuantity(); i++)
			{
				WIRE_DELETE (*pStateList)[i];
			}

			mMaterialStates.Remove(pMaterial);
		}

		WIRE_DELETE pMaterial;
		return NULL;
	}

	mMaterials.Insert(pName, pMaterial);
	mStatistics.MaterialCount++;

	return pMaterial;
}

//----------------------------------------------------------------------------
Texture2D* Importer::ParseTexture(rapidxml::xml_node<>* pXmlNode,
	Material::BlendMode& blendMode)
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
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);	
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
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);	
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
		else if (System::Strcmp("BlendMode", attr->name()) == 0)
		{
			if (System::Strcmp("Replace", attr->value()) == 0)
			{
				blendMode = Material::BM_REPLACE;
			}
			else if (System::Strcmp("Modulate", attr->value()) == 0)
			{
				blendMode = Material::BM_MODULATE;
			}
			else if (System::Strcmp("Pass", attr->value()) == 0)
			{
				blendMode = Material::BM_PASS;
			}
			else if (System::Strcmp("Blend", attr->value()) == 0)
			{
				blendMode = Material::BM_BLEND;
			}
			else if (System::Strcmp("Decal", attr->value()) == 0)
			{
				blendMode = Material::BM_DECAL;
			}
		}
	}

	Image2D* pImage = LoadPNG(pName, (mipmapCount > 1));
	if (!pImage)
	{
		return NULL;
	}

	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	mStatistics.TextureCount++;
	pTexture->SetFilterType(filter);
	pTexture->SetWrapType(0, warp);
	pTexture->SetWrapType(1, warp);
	pTexture->SetAnisotropyValue(static_cast<Float>(anisoLevel));

	mTextures.Insert(pName, pTexture);

	return pTexture;
}