#include "Importer.h"

#include "BulletUtils.h"
#include "PicoPNG.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "WireEffect.h"
#include "WireImage2D.h"
#include "WireLight.h"
#include "WireMesh.h"
#include "WireNodeLight.h"
#include "WireQuaternion.h"
#include "WireStandardMesh.h"
#include "WireStateAlpha.h"
#include "WireStateCull.h"
#include "WireStateFog.h"
#include "WireStateMaterial.h"
#include "WireStateWireframe.h"
#include "WireStateZBuffer.h"
#include "WireTStack.h"
#include "WireVertexAttributes.h"

using namespace Wire;

//----------------------------------------------------------------------------
Importer::Importer(const Char* pPath, Options* pOptions)
	:
	mpPath(pPath),
	mStaticSpatials(0, 100)
{
	mpOptions = pOptions ? pOptions : &mDefaultOptions;
}

//----------------------------------------------------------------------------
Node* Importer::LoadSceneFromXml(const Char* pFilename, TArray<CameraPtr>*
	pCameras, btDynamicsWorld* pPhysicsWorld)
{
	ResetStatistics();
	mpCameras = pCameras;

	String path = String(mpPath) + String(pFilename);
	Int xmlSize;
	Char* pXml = Load(static_cast<const Char*>(path), xmlSize);
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

	if (doc.first_node())
	{
		for (rapidxml::xml_node<>* pChild = doc.first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			Traverse(pChild, pRoot);
		}
	}

	WIRE_DELETE[] pXmlNullTerminated;

	mMaterials.RemoveAll();
	mMaterialStates.RemoveAll();
	mMeshes.RemoveAll();
	mTextures.RemoveAll();
	mLights.RemoveAll();

	pRoot->UpdateRS();

	if (mStaticSpatials.GetQuantity() > 0)
	{
		pRoot->UpdateGS();
		InitializeStaticSpatials(mStaticSpatials, mpOptions->
			PrepareSceneForStaticBatching, mpOptions->
			DuplicateSharedMeshesWhenPreparingSceneForStaticBatching);
		mStaticSpatials.RemoveAll();
	}

	if (pPhysicsWorld != NULL)
	{
		RegisterColliders(mColliders, pPhysicsWorld);
		mColliders.RemoveAll();
	}

	return pRoot;
}

//----------------------------------------------------------------------------
Image2D* Importer::LoadPNG(const Char* pFilename, Bool hasMipmaps,
	Buffer::UsageType usage)
{
	Int pngSize;
	UChar* pPNG = reinterpret_cast<UChar*>(Load(pFilename, pngSize));
	if (!pPNG)
	{
		return NULL;
	}

	Image2D* pImage2D = DecodePNG(pPNG, pngSize, hasMipmaps, usage);
	WIRE_DELETE[] pPNG;

	return pImage2D;
}

//----------------------------------------------------------------------------
Texture2D* Importer::LoadTexture2D(const Char* pFilename, Bool hasMipmaps,
	Buffer::UsageType usage)
{
	Image2D* pImage = LoadPNG(pFilename, hasMipmaps, usage);
	if (!pImage)
	{
		return NULL;
	}

	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	WIRE_ASSERT(pTexture);
	return pTexture;
}

//----------------------------------------------------------------------------
Image2D* Importer::DecodePNG(const UChar* pPNG, size_t pngSize,
	Bool hasMipmaps, Buffer::UsageType usage)
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
		hasMipmaps, usage);

	return pImage;
}

//----------------------------------------------------------------------------
Text* Importer::CreateText(const Char* pFilename, UInt width, UInt height,
	UInt maxLength)
{
	// Init FreeType lib and font
	Int fileSize;
	UChar* pMemFile = reinterpret_cast<UChar*>(Load(pFilename, fileSize));
	if (!pMemFile)
	{
		return NULL;
	}

	FT_Library library;
	
	FT_Error error = FT_Init_FreeType(&library);
	if (error != 0)
	{
		WIRE_ASSERT(false /* Could not initialize freetype lib */);
		WIRE_DELETE[] pMemFile;
		return NULL;
	}

	FT_Face face;
	error = FT_New_Memory_Face(library, pMemFile, fileSize, 0, &face);
	if (error)
	{
		if (error == FT_Err_Unknown_File_Format)
		{
			WIRE_ASSERT(false /* Font format is not supported */);
		}
		else
		{
			WIRE_ASSERT(false /* Could not create face from memory */);
		}

		FT_Done_FreeType(library);
		WIRE_DELETE[] pMemFile;
		return NULL;
	}

	error = FT_Set_Pixel_Sizes(face, width, height);
	if (error)
	{
		// Usually, an error occurs with a fixed-size font format (like FNT or
		// PCF) when trying to set the pixel size to a value that is
		// not listed in the face->fixed_sizes array
		WIRE_ASSERT(false /* Could not set pixel size */);
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		WIRE_DELETE[] pMemFile;
		return NULL;
	}

	// Calculate the required font texture size from the font size
	const UInt totalCharCount = 128;
	UInt texWidth = 32;
	UInt texHeight = 32;
	FT_GlyphSlot slot = face->glyph;

	Bool fitsInTexture = false;
	while (!fitsInTexture)
	{
		if (texWidth > 1024 || texHeight > 1024)
		{
			return NULL;
		}

		UInt wc = 0;
		Int penX = 0;
		Int penY = height;
		fitsInTexture = true;

		for (UInt i = 0; i < totalCharCount; i++)
		{
			FT_UInt glyphIndex = FT_Get_Char_Index(face, wc++);
			if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER))
			{
				continue;
			}

			Int advance = slot->advance.x >> 6;
			advance = advance > slot->bitmap.width ? advance : slot->bitmap.width;
			if (penX+advance >= static_cast<Int>(texWidth))
			{
				penX = 0;
				penY += height;
				if (penY+height >= texHeight)
				{
					if (texWidth > texHeight)
					{
						texHeight = texHeight << 1;
					}
					else
					{
						texWidth = texWidth << 1;
					}

					fitsInTexture = false;
					break;
				}
			}

			penX += advance;
		}
	}

	UChar* const pDst = WIRE_NEW UChar[texWidth * texHeight * 4];
	System::Memset(pDst, 0, texWidth * texHeight * 4);
	TArray<Vector2F> uvs(totalCharCount*4);
	TArray<Vector4F> charSizes(totalCharCount);

	Int penY = height;
	Int penX = 0;
	for (UInt wc = 0; wc < totalCharCount; wc++)
	{
		FT_UInt glyphIndex = FT_Get_Char_Index(face, wc);
		if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER))
		{
			uvs.Append(Vector2F(0, 0));
			uvs.Append(Vector2F(1, 0));
			uvs.Append(Vector2F(1, 1));
			uvs.Append(Vector2F(0, 1));
			Float fWidth = static_cast<Float>(width);
			Float fHeight = static_cast<Float>(height);
			charSizes.Append(Vector4F(fWidth, fHeight, fWidth, 0));
			continue;
		}

		Int advance = slot->advance.x >> 6;
		advance = advance > slot->bitmap.width ? advance : slot->bitmap.width;
		if (penX+advance >= static_cast<Int>(texWidth))
		{
			penX = 0;
			penY += height;
			if (penY+height >= texHeight)
			{
				if (texWidth > texHeight)
				{
					texHeight = texHeight << 1;
				}
				else
				{
					texWidth = texWidth << 1;
				}

				fitsInTexture = false;
				break;
			}
		}

		Int offset = penX + slot->bitmap_left;
		Int top = penY - slot->bitmap_top;
		FT_Bitmap& rBitmap = slot->bitmap;

		Float u0 = static_cast<Float>(offset)/texWidth;
		Float v0 = static_cast<Float>(top)/texHeight;
		Float u1 = static_cast<Float>(offset+rBitmap.width)/texWidth;
		Float v1 = static_cast<Float>(top+rBitmap.rows)/texHeight;
		uvs.Append(Vector2F(u0, v0));
		uvs.Append(Vector2F(u1, v0));
		uvs.Append(Vector2F(u1, v1));
		uvs.Append(Vector2F(u0, v1));

		Float cWidth = static_cast<Float>(rBitmap.width);
		Float cHeight = static_cast<Float>(rBitmap.rows);
		Float cStride = static_cast<Float>(slot->advance.x >> 6);
		Float cOffsetY = static_cast<Float>(slot->bitmap.rows - slot->bitmap_top);
		charSizes.Append(Vector4F(cWidth, cHeight, cStride, cOffsetY));

		Int q = 0;
		for (Int j = top; j < (top + rBitmap.rows); j++, q++)
		{
			Int p = 0;
			for (Int i = offset; i < (offset + rBitmap.width); i++, p++)
			{
				UChar pixel = rBitmap.buffer[q*rBitmap.width + p];
				WIRE_ASSERT(((j*texWidth + i) < (texWidth * texHeight)) && ((j*texWidth + i) >= 0));
				pDst[(j*texWidth + i)*4] = 0xFF;
				pDst[(j*texWidth + i)*4+1] = 0xFF;
				pDst[(j*texWidth + i)*4+2] = 0xFF;
				pDst[(j*texWidth + i)*4+3] = pixel;
			}
		}

		penX += advance;
		WIRE_ASSERT((slot->advance.y >> 6) <= static_cast<Int>(height));
	}

	Image2D* pImage = WIRE_NEW Image2D(Image2D::FM_RGBA8888, texWidth, texHeight, pDst, false);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetFilterType(Texture2D::FT_NEAREST);
 	Text* pText = WIRE_NEW Text(height, pTexture, uvs, charSizes, maxLength);

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	WIRE_DELETE[] pMemFile;
	return pText;
}

//----------------------------------------------------------------------------
Char* Importer::Load(const Char* pFilename, Int& rSize)
{
	Char* pBuffer;
	Bool hasSucceeded;
	hasSucceeded = System::Load(pFilename, pBuffer, rSize);
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
	WIRE_ASSERT(pFilename);
	if (!pFilename)
	{
		return NULL;
	}

	String path = String(mpPath) + String(pFilename);
	Char* pBuffer = Load(static_cast<const Char*>(path), rSize);
	if (!pBuffer)
	{
		return NULL;
	}

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
UShort* Importer::Load16(const Char* pFilename, Int& rSize, Bool isBigEndian)
{
	WIRE_ASSERT(pFilename);
	if (!pFilename)
	{
		return NULL;
	}

	String path = String(mpPath) + String(pFilename);
	Char* pBuffer = Load(static_cast<const Char*>(path), rSize);
	if (!pBuffer)
	{
		return NULL;
	}

	UShort* pBuffer16 = reinterpret_cast<UShort*>(pBuffer);
	if (System::IsBigEndian() != isBigEndian)
	{
		if (!pBuffer16 || (rSize & 1) != 0)
		{
			WIRE_ASSERT(false);
			return NULL;
		}

		for (Int i = 0; i < rSize; i+=2)
		{
			UChar i0 = pBuffer[i+0];
			pBuffer[i+0] = pBuffer[i+1];
			pBuffer[i+1] = i0;
		}
	}

	return pBuffer16;
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
	mStatistics.MeshCount = 0;
	mStatistics.VertexBufferCount = 0;
	mStatistics.IndexBufferCount = 0;
	mStatistics.ColliderCount = 0;
}

//----------------------------------------------------------------------------
void Importer::Traverse(rapidxml::xml_node<>* pXmlNode, Node* pParent)
{
	if (Is("Assets", pXmlNode->name()))
	{
		ParseAssets(pXmlNode);
		return;
	}

	if (Is("Skybox", pXmlNode->name()))
	{
		NodeSkybox* pSkybox = ParseSkybox(pXmlNode);
		if (pSkybox)
		{
			pParent->AttachChild(pSkybox);
		}

		return;
	}

	if (Is("Text", pXmlNode->name()))
	{
		Text* pText = ParseText(pXmlNode);
		if (pText)
		{
			pParent->AttachChild(pText);
		}

		return;
	}

	if (Is("Leaf", pXmlNode->name()))
	{
		Geometry* pGeo = ParseLeaf(pXmlNode);
		if (pGeo)
		{
			pParent->AttachChild(pGeo);
		}

		return;
	}

	WIRE_ASSERT(Is("Node", pXmlNode->name()));
	Node* pNode = ParseNode(pXmlNode);
	pParent->AttachChild(pNode);

	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			ParseComponent(pChild, pNode);

			if (Is("Node", pChild->name()) || Is("Leaf", pChild->name()) ||
				Is("Text", pChild->name()) || Is("Skybox", pChild->name()))
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
		if (Is(pName, attr->name()))
		{
			return attr->value();
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------
Bool Importer::HasValue(rapidxml::xml_node<>* pXmlNode, const Char* pName)
{
	return GetValue(pXmlNode, pName) != NULL;
}

//----------------------------------------------------------------------------
Bool Importer::IsBigEndian(rapidxml::xml_node<>* pXmlNode)
{
	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (Is("LittleEndian", attr->name()))
		{
			Char* pValue = attr->value();
			if (pValue)
			{
				if (*pValue == 'y' || *pValue == '1')
				{
					return false;
				}
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------
Bool Importer::Is16Bit(rapidxml::xml_node<>* pXmlNode)
{
	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (Is("16bit", attr->name()))
		{
			Char* pValue = attr->value();
			if (pValue)
			{
				if (*pValue == '1')
				{
					return true;
				}
			}
		}
	}

	return false;
}

//----------------------------------------------------------------------------
Buffer::UsageType Importer::GetUsageType(rapidxml::xml_node<>* pXmlNode)
{
	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (Is("Usage", attr->name()))
		{
			Char* pValue = attr->value();
			if (pValue)
			{
				if (Is("STATIC", pValue))
				{
					return Buffer::UT_STATIC;
				}
				else if (Is("DYNAMIC", pValue))
				{
					return Buffer::UT_DYNAMIC;
				}
				else if (Is("DYNAMIC_WRITE_ONLY", pValue))
				{
					return  Buffer::UT_DYNAMIC_WRITE_ONLY;
				}
				else if (Is("STATIC_DISCARD_ON_BIND", pValue))
				{
					return  Buffer::UT_STATIC_DISCARD_ON_BIND;
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
UInt Importer::GetUInt(rapidxml::xml_node<>* pXmlNode, const Char* pName)
{
	Char* pUInt = GetValue(pXmlNode, pName);
	UInt i = 0;
	if (pUInt)
	{
		Int n;
		n = sscanf(pUInt, "%d", &i);
		WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
	}

	return i;
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
	pName, Bool& rHasValue)
{
	ColorRGB c = ColorRGB::WHITE;
	rHasValue = false;
	Char* pCol = GetValue(pXmlNode, pName);
	if (pCol)
	{
		Int n;
		n = sscanf(pCol, "%f, %f, %f", &c.R(), &c.G(), &c.B());
		WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
		rHasValue = true;
	}

	return c;
}

//----------------------------------------------------------------------------
ColorRGBA Importer::GetColorRGBA(rapidxml::xml_node<>* pXmlNode, const Char*
	pName, Bool& rHasValue)
{
	ColorRGBA c = ColorRGBA::WHITE;
	rHasValue = false;
	Char* pCol = GetValue(pXmlNode, pName);
	if (pCol)
	{
		Int n;
		n = sscanf(pCol, "%f, %f, %f, %f", &c.R(), &c.G(), &c.B(), &c.A());
		WIRE_ASSERT_NO_SIDEEFFECTS(n == 4);
		rHasValue = true;
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
void Importer::ParseAssets(rapidxml::xml_node<>* pXmlNode)
{
	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			if (Is("Meshes", pChild->name()))
			{
				for (rapidxml::xml_node<>* pMesh = pChild->first_node();
					pMesh; pMesh = pMesh->next_sibling())
				{
					ParseMesh(pMesh);
				}
			}
			else if (Is("Materials", pChild->name()))
			{
				for (rapidxml::xml_node<>* pMaterial = pChild->first_node();
					pMaterial; pMaterial = pMaterial->next_sibling())
				{
					ParseMaterial(pMaterial);
				}
			}
			else if (Is("Lights", pChild->name()))
			{
				for (rapidxml::xml_node<>* pLight = pChild->first_node();
					pLight; pLight = pLight->next_sibling())
				{
					ParseLight(pLight);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void Importer::ParseCollider(rapidxml::xml_node<>* pXmlNode, Spatial* pSpatial)
{
	UpdateGS(pSpatial);

	Char* pShapeName = GetValue(pXmlNode, "Shape");
	WIRE_ASSERT(pShapeName);

	btCollisionShape* pCollisionShape = NULL;
	if (Is("Box", pShapeName))
	{
		// FIXME: center is not being used!
		Vector3F center = Vector3F::ZERO;
		Vector3F size = Vector3F::ONE;

		for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();
			attr; attr = attr->next_attribute())
		{
			if (Is("Center", attr->name()))
			{
				Int n;
				n = sscanf(attr->value(), "%f, %f, %f", &center.X(), &center.Y(), &center.Z());
				WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
			}
			else if (Is("Size", attr->name()))
			{
				Int n;
				n = sscanf(attr->value(), "%f, %f, %f", &size.X(), &size.Y(), &size.Z());
				WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
			}
		}

		pCollisionShape = WIRE_NEW btBoxShape(BulletUtils::Convert(size * 0.5f));
	}
	else if (Is("Mesh", pShapeName))
	{
		rapidxml::xml_node<>* pFirstChild = pXmlNode->first_node();

		WIRE_ASSERT(pFirstChild /* Mesh collider has no child */);
		WIRE_ASSERT(Is("Mesh", pFirstChild->name()) /* First child of mesh collider is not a mesh */);
		
		Mesh* pMesh = ParseMesh(pFirstChild);
		btTriangleIndexVertexArray* pTriangleIndexVertexArray = BulletUtils::Convert(pMesh);

		pCollisionShape = WIRE_NEW btBvhTriangleMeshShape(pTriangleIndexVertexArray, false);
	}
	else 
	{
		WIRE_ASSERT(false /* Collider shape not supported yet! */);
	}

	if (!pCollisionShape)
	{
		return;
	}

	Collider* pCollider = WIRE_NEW Collider(pCollisionShape);
	pSpatial->AttachController(pCollider);
	mColliders.Append(pCollider);

	mStatistics.ColliderCount++;
}

//----------------------------------------------------------------------------
Light* Importer::ParseLight(rapidxml::xml_node<>* pXmlNode)
{
	Char* pName = GetValue(pXmlNode, "Name");
	if (pName)
	{
		if (mpOptions->AssetsWithEqualNamesAreIdentical)
		{
			LightPtr* pValue = mLights.Find(pName);
			if (pValue)
			{
				WIRE_ASSERT(*pValue);
				return *pValue;
			}
		}
	}

	Char* pType = GetValue(pXmlNode, "Type");
	WIRE_ASSERT(pType);
	Light* pLight = WIRE_NEW Light;
	Light::LightType lt = Light::LT_POINT;

	if (Is("Point", pType))
	{
		lt = Light::LT_POINT;
	}
	else if (Is("Directional", pType))
	{
		lt = Light::LT_DIRECTIONAL;
	}
	else if (Is("Spot", pType))
	{
		lt = Light::LT_SPOT;
		if (HasValue(pXmlNode, "Angle"))
		{
			pLight->Angle = GetFloat(pXmlNode, "Angle");
		}

		if (HasValue(pXmlNode, "Exp"))
		{
			pLight->Exponent = GetFloat(pXmlNode, "Exp");
		}
	}

	pLight->Type = lt;
	Bool hasValue;
	ColorRGB ambient = GetColorRGB(pXmlNode, "Ambient", hasValue); 
	if (hasValue)
	{
		pLight->Ambient = ambient; 
	}

	ColorRGB color = GetColorRGB(pXmlNode, "Color", hasValue);
	if (hasValue)
	{
		pLight->Color = color;
	}

	if (pName)
	{
		mLights.Insert(pName, pLight);
	}

	return pLight;
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

	pCam->SetFrustum(fov, 640.0F / 480.0F , near, far);
	mpCameras->Append(pCam);
}

//----------------------------------------------------------------------------
void Importer::ParseTransformation(rapidxml::xml_node<>* pXmlNode, Spatial*
	pSpatial)
{
	Vector3F t = Vector3F::ZERO;
	QuaternionF r = QuaternionF::IDENTITY;
	Vector3F s = Vector3F::ONE;
	
	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (Is("Pos", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f", &t.X(), &t.Y(), &t.Z());
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
		}
		else if (Is("Rot", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f, %f", &r.W(), &r.X(),
				&r.Y(), &r.Z());
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 4);
		}
		else if (Is("Scale", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f", &s.X(), &s.Y(), &s.Z());
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 3);
		}
		else if (Is("Static", attr->name()))
		{
			Int n;
			Int st;
			n = sscanf(attr->value(), "%d", &st);
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
			if (st != 0)
			{
				mStaticSpatials.Append(pSpatial);
			}
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

	if ((s.X() != 1.0F) || (s.Y() != 1.0F) || (s.Z() != 1.0F))
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

//---------------------------------------------------------------------------
Node* Importer::ParseNode(rapidxml::xml_node<>* pXmlNode)
{
	Node* pNode = NULL;

	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		if (Is("LightNode", pChild->name()))
		{
			pNode = WIRE_NEW NodeLight;
			ParseComponents(pChild, pNode);
			WIRE_ASSERT(pNode->GetLightQuantity() == 1);
			Light* pLight = pNode->GetLight();
			NodeLight* pLightNode = DynamicCast<NodeLight>(pNode);
			WIRE_ASSERT(pLightNode);
			pLightNode->SetLight(pLight);
			pNode->DetachLight(pLight);
			break;
		}
	}

	if (!pNode)
	{
		pNode = WIRE_NEW Node;
	}

	mStatistics.NodeCount++;

	Char* pName = GetValue(pXmlNode, "Name");
	if (pName)
	{
		pNode->SetName(pName);
	}

	ParseTransformationAndComponents(pXmlNode, pNode);

	return pNode;
}

//----------------------------------------------------------------------------
Geometry* Importer::ParseLeaf(rapidxml::xml_node<>* pXmlNode)
{
	Mesh* pMesh = NULL;
	Material* pMaterial = NULL;
	UInt subMeshIndex = GetUInt(pXmlNode, "SubMeshIndex");

	if (pXmlNode->first_node())
	{
		for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
			pChild = pChild->next_sibling())
		{
			if (Is("Mesh", pChild->name()))
			{
				pMesh = ParseMesh(pChild, subMeshIndex);
			}
			else if (Is("Material", pChild->name()))
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
	WIRE_ASSERT(pGeo);
	mStatistics.GeometryCount++;
	Char* pName = GetValue(pXmlNode, "Name");
	if (pName)
	{
		pGeo->SetName(pName);
	}

	if (pMaterial)
	{
		TArray<StatePtr>* pStateList = mMaterialStates.Find(pMaterial);
		if (pStateList)
		{
			for (UInt i = 0; i < pStateList->GetQuantity(); i++)
			{
				pGeo->AttachState((*pStateList)[i]);
			}
		}
	}

	ParseTransformationAndComponents(pXmlNode, pGeo);

	return pGeo;
}

//----------------------------------------------------------------------------
Text* Importer::ParseText(rapidxml::xml_node<>* pXmlNode)
{
	Int width = 8;
	Int height = 8;
	Int maxLength = -1;

	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (Is("Width", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%d", &width);
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
		}
		else if (Is("Height", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%d", &height);
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
		}
		else if (Is("MaxLength", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%d", &maxLength);
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);
		}
	}

	Char* pString = GetValue(pXmlNode, "String");
	if (maxLength < 0)
	{
		maxLength = System::Strlen(pString);
	}

	Text* pText = NULL;
	Char* pFontName = GetValue(pXmlNode, "Font");
	if (pFontName)
	{
		String path = String(mpPath) + String(pFontName);
		pText = CreateText(path, width, height, maxLength);
	}
	else
	{
		pText = StandardMesh::CreateText(maxLength);
	}

	if (!pText)
	{
		WIRE_ASSERT(false);
		return NULL;
	}

	Bool hasValue;
	ColorRGBA color = GetColorRGBA(pXmlNode, "Color", hasValue);
	if (hasValue)
	{
		pText->SetColor(color);
	}

	if (pString)
	{
		Bool isOk = pText->Set(pString);
		WIRE_ASSERT_NO_SIDEEFFECTS(isOk /* Wire::Text::Set() failed */);
	}

	mStatistics.GeometryCount++;
	Char* pName = GetValue(pXmlNode, "Name");
	if (pName)
	{
		pText->SetName(pName);
	}

	ParseTransformationAndComponents(pXmlNode, pText);

	return pText;
}

//----------------------------------------------------------------------------
NodeSkybox* Importer::ParseSkybox(rapidxml::xml_node<>* pXmlNode)
{
	if (!pXmlNode->first_node())
	{
		return NULL;
	}

	Float scale = GetFloat(pXmlNode, "Scale");
	scale = (scale == 0) ? 100.0F : scale;

	Texture2D* pPosZ = ParseSkyboxTexture("PosZ", pXmlNode);
	Texture2D* pNegZ = ParseSkyboxTexture("NegZ", pXmlNode);
	Texture2D* pPosX = ParseSkyboxTexture("PosX", pXmlNode);
	Texture2D* pNegX = ParseSkyboxTexture("NegX", pXmlNode);
	Texture2D* pPosY = ParseSkyboxTexture("PosY", pXmlNode);
	Texture2D* pNegY = ParseSkyboxTexture("NegY", pXmlNode);

	NodeSkybox* pSkyBox = WIRE_NEW NodeSkybox(pPosZ, pNegZ, pPosX, pNegX,
		pPosY, pNegY, scale);

	ParseComponents(pXmlNode, pSkyBox);

	return pSkyBox;
}

//----------------------------------------------------------------------------
Texture2D* Importer::ParseSkyboxTexture(const Char* pName,
	rapidxml::xml_node<>* pXmlNode)
{
	Texture2D* pTexture = NULL;
	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		if (Is(pName, pChild->name()))
		{
			for (rapidxml::xml_node<>* pGrandChild = pChild->first_node();
				pGrandChild; pGrandChild = pGrandChild->next_sibling())
			{
				if (Is("Texture", pGrandChild->name()))
				{
					Material::BlendMode bm = Material::BM_REPLACE;
					return ParseTexture(pGrandChild, bm);
				}
			}
		}
	}

	return pTexture;
}

//----------------------------------------------------------------------------
void Importer::ParseComponent(rapidxml::xml_node<>* pXmlNode, Spatial*
	pSpatial)
{
	State* pState = ParseRenderStates(pXmlNode);
	if (pState)
	{
		pSpatial->AttachState(pState);
	}
	else if (Is("Camera", pXmlNode->name()))
	{
		ParseCamera(pXmlNode, pSpatial);
	}
	else if (Is("Light", pXmlNode->name()))
	{
		Light* pLight =	ParseLight(pXmlNode);
		if (pLight)
		{
			pSpatial->AttachLight(pLight);
		}
	}
	else if (Is("Collider", pXmlNode->name()))
	{
		ParseCollider(pXmlNode, pSpatial);
	}
}

//----------------------------------------------------------------------------
void Importer::ParseComponents(rapidxml::xml_node<>* pXmlNode, Spatial*
	pSpatial)
{
	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		ParseComponent(pChild, pSpatial);
	}
}

//----------------------------------------------------------------------------
void Importer::ParseTransformationAndComponents(rapidxml::xml_node<>* pXmlNode,
	Spatial* pSpatial)
{
	ParseTransformation(pXmlNode, pSpatial);
	ParseComponents(pXmlNode, pSpatial);
}

//----------------------------------------------------------------------------
State* Importer::ParseRenderStates(rapidxml::xml_node<>* pXmlNode)
{
	if (Is("MaterialState", pXmlNode->name()))
	{
		StateMaterial* pMaterialState = WIRE_NEW StateMaterial;
		Bool hasValue;	
		ColorRGBA ambient = GetColorRGBA(pXmlNode, "Ambient", hasValue);
		if (hasValue)
		{
			pMaterialState->Ambient = ambient;
		}

		return pMaterialState;
	}
	else if (Is("AlphaState", pXmlNode->name()))
	{
		StateAlpha* pAlphaState = WIRE_NEW StateAlpha;
		pAlphaState->BlendEnabled = GetBool(pXmlNode, "Enabled");

		Char* pSrcBlend = GetValue(pXmlNode, "SrcBlendMode");
		if (pSrcBlend)
		{
			if (Is("ZERO", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ZERO;
			}
			else if (Is("ONE", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE;
			}
			else if (Is("DST_COLOR", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_DST_COLOR;
			}
			else if (Is("ONE_MINUS_DST_COLOR", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE_MINUS_DST_COLOR;
			}
			else if (Is("SRC_ALPHA", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_SRC_ALPHA;
			}
			else if (Is("ONE_MINUS_SRC_ALPHA", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE_MINUS_SRC_ALPHA;
			}
			else if (Is("DST_ALPHA", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_DST_ALPHA;
			}
			else if (Is("ONE_MINUS_DST_ALPHA", pSrcBlend))
			{
				pAlphaState->SrcBlend = StateAlpha::SBM_ONE_MINUS_DST_ALPHA;
			}
		}

		Char* pDstBlend = GetValue(pXmlNode, "DstBlendMode");
		if (pDstBlend)
		{
			if (Is("ZERO", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ZERO;
			}
			else if (Is("ONE", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE;
			}
			else if (Is("SRC_COLOR", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_SRC_COLOR;
			}
			else if (Is("ONE_MINUS_SRC_COLOR", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE_MINUS_SRC_COLOR;
			}
			else if (Is("SRC_ALPHA", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_SRC_ALPHA;
			}
			else if (Is("ONE_MINUS_SRC_ALPHA", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE_MINUS_SRC_ALPHA;
			}
			else if (Is("DST_ALPHA", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_DST_ALPHA;
			}
			else if (Is("ONE_MINUS_DST_ALPHA", pDstBlend))
			{
				pAlphaState->DstBlend = StateAlpha::DBM_ONE_MINUS_DST_ALPHA;
			}
		}

		return pAlphaState;
	}
	else if (Is("ZBufferState", pXmlNode->name()))
	{
		StateZBuffer* pZBufferState = WIRE_NEW StateZBuffer;
		pZBufferState->Enabled = GetBool(pXmlNode, "Enabled");
		pZBufferState->Writable = GetBool(pXmlNode, "Writable");

		Char* pCmpFunc = GetValue(pXmlNode, "CmpFunc");
		if (pCmpFunc)
		{
			if (Is("NEVER", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_NEVER;
			}
			else if (Is("LESS", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_LESS;
			}
			else if (Is("EQUAL", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_EQUAL;
			}
			else if (Is("LEQUAL", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_LEQUAL;
			}
			else if (Is("GREATER", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_GREATER;
			}
			else if (Is("NOTEQUAL", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_NOTEQUAL;
			}
			else if (Is("GEQUAL", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_GEQUAL;
			}
			else if (Is("ALWAYS", pCmpFunc))
			{
				pZBufferState->Compare = StateZBuffer::CF_ALWAYS;
			}
		}

		return pZBufferState;
	}
	else if (Is("CullState", pXmlNode->name()))
	{
		StateCull* pCullState = WIRE_NEW StateCull;
		pCullState->Enabled = GetBool(pXmlNode, "Enabled");

		Char* pMode = GetValue(pXmlNode, "Mode");
		if (pMode)
		{
			if (Is("OFF", pMode))
			{
				pCullState->CullFace = StateCull::CM_OFF;
			}
			else if (Is("BACK", pMode))
			{
				pCullState->CullFace = StateCull::CM_BACK;
			}
			else if (Is("FRONT", pMode))
			{
				pCullState->CullFace = StateCull::CM_FRONT;
			}
		}

		return pCullState;
	}
	else if (Is("WireframeState", pXmlNode->name()))
	{
		StateWireframe* pWireframeState = WIRE_NEW StateWireframe;
		pWireframeState->Enabled = GetBool(pXmlNode, "Enabled");

		return pWireframeState;
	}
	else if (Is("FogState", pXmlNode->name()))
	{
		StateFog* pFogState = WIRE_NEW StateFog;
		pFogState->Enabled = GetBool(pXmlNode, "Enabled");
		Bool hasValue;
		ColorRGB color = GetColorRGB(pXmlNode, "Color", hasValue);
		if (hasValue)
		{
			pFogState->Color = color;
		}

		pFogState->Start = GetFloat(pXmlNode, "Start");
		pFogState->End = GetFloat(pXmlNode, "End");

		Char* pFunc = GetValue(pXmlNode, "Func");
		if (pFunc)
		{
			if (Is("LINEAR", pFunc))
			{
				pFogState->DensityFunc = StateFog::DF_LINEAR;
			}
			else if (Is("EXP", pFunc))
			{
				pFogState->DensityFunc = StateFog::DF_EXP;
			}
			else if (Is("EXPSQR", pFunc))
			{
				pFogState->DensityFunc = StateFog::DF_EXPSQR;
			}
		}

		return pFogState;
	}

	return NULL;
}

//----------------------------------------------------------------------------
Mesh* Importer::ParseMesh(rapidxml::xml_node<>* pXmlNode, UInt subMeshIndex)
{
	Char* pName = GetValue(pXmlNode, "Name");
	if (!pName)
	{
		WIRE_ASSERT(false /* Mesh has no name */);
		return NULL;
	}

	if (mpOptions->AssetsWithEqualNamesAreIdentical)
	{
		TArray<MeshPtr>* pValue = mMeshes.Find(pName);
		if (pValue)
		{
			WIRE_ASSERT(pValue);
			WIRE_ASSERT(subMeshIndex < pValue->GetQuantity());
			return (*pValue)[subMeshIndex];
		}
	}

	Char* pVerticesFileName = NULL;
	Char* pIndicesFileName = NULL;
	Char* pNormalsFileName = NULL;
	Char* pColorsFileName = NULL;
	TArray<Char*> uvSetNames(8,8);
	Bool isVertexBufferBigEndian = true;
	Bool isIndexBufferBigEndian = true;
	Bool isNormalsBigEndian = true;
	Bool isColorsBigEndian = true;
	Bool isIndexBuffer16Bit = false;
	TArray<Bool> uvBigEndian(8,8);
	Buffer::UsageType vertexBufferUsage = Buffer::UT_STATIC;
	Buffer::UsageType indexBufferUsage = Buffer::UT_STATIC;

	for (rapidxml::xml_node<>* pChild = pXmlNode->first_node(); pChild;
		pChild = pChild->next_sibling())
	{
		if (!pVerticesFileName && Is("Vertices", pChild->name()))
		{
			pVerticesFileName = GetValue(pChild, "Name");
			isVertexBufferBigEndian = IsBigEndian(pChild);
			vertexBufferUsage = GetUsageType(pChild);
		}
		else if (!pIndicesFileName && Is("Indices", pChild->name()))
		{
			pIndicesFileName = GetValue(pChild, "Name");
			isIndexBufferBigEndian = IsBigEndian(pChild);
			indexBufferUsage = GetUsageType(pChild);
			isIndexBuffer16Bit = Is16Bit(pChild);
		}
		else if (!pNormalsFileName && Is("Normals", pChild->name()))
		{
			pNormalsFileName = GetValue(pChild, "Name");
			isNormalsBigEndian = IsBigEndian(pChild);
		}
		else if (!pColorsFileName && Is("Colors", pChild->name()))
		{
			pColorsFileName = GetValue(pChild, "Name");
			isColorsBigEndian = IsBigEndian(pChild);
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

	if (!pVerticesFileName || !pIndicesFileName)
	{
		WIRE_ASSERT(false /* Mesh has no vertices or indices files */);
		return NULL;
	}

	VertexBuffer* pVertexBuffer = LoadVertexBufferFromFiles(pVerticesFileName,
		isVertexBufferBigEndian, vertexBufferUsage, pNormalsFileName,
		isNormalsBigEndian, pColorsFileName, isColorsBigEndian, uvSetNames,
		uvBigEndian);

	IndexBuffer* pIndexBuffer = LoadIndexBufferFromFile(pIndicesFileName,
		isIndexBufferBigEndian, indexBufferUsage, isIndexBuffer16Bit);

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	mStatistics.MeshCount++;
	TArray<MeshPtr> pSubMeshes; // = WIRE_NEW TArray<MeshPtr>;
	pSubMeshes.Append(pMesh);

	for (rapidxml::xml_node<>* pSubNode = pXmlNode->first_node();
		pSubNode; pSubNode = pSubNode->next_sibling())
	{
		if (Is("SubMeshes", pSubNode->name()))
		{
			for (rapidxml::xml_node<>* pChild = pSubNode->first_node();
				pChild;	pChild = pChild->next_sibling())
			{
				WIRE_ASSERT(HasValue(pChild, "Index"));
				UInt i = GetUInt(pChild, "Index");
				WIRE_ASSERT(HasValue(pChild, "StartIndex"));
				UInt startIndex = GetUInt(pChild, "StartIndex");
				WIRE_ASSERT(HasValue(pChild, "IndexCount"));
				UInt indexCount = GetUInt(pChild, "IndexCount");

				Mesh* pSubMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer,
					startIndex, indexCount);
				pSubMeshes.SetElement(i, pSubMesh);

				if (i > 0)
				{
					mStatistics.MeshCount++;
				}
			}
		}
	}

	mMeshes.Insert(pName, pSubMeshes);
	return pSubMeshes[subMeshIndex];
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

	if (mpOptions->AssetsWithEqualNamesAreIdentical)
	{
		MaterialPtr* pValue = mMaterials.Find(pName);
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
			if (Is("Texture", pChild->name()))
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
					TArray<StatePtr>* pStateList = mMaterialStates.Find(pMaterial);
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
						TArray<StatePtr> stateList;
						stateList.Append(pState);
						mMaterialStates.Insert(pMaterial, stateList);
					}
				}
			}
		}
	}

	if (pMaterial->GetTextureQuantity() == 0)
	{
		TArray<StatePtr>* pStateList = mMaterialStates.Find(pMaterial);
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

	if (mpOptions->AssetsWithEqualNamesAreIdentical)
	{
		Texture2DPtr* pValue = mTextures.Find(pName);
		if (pValue)
		{
			WIRE_ASSERT(*pValue);
			return *pValue;
		}
	}

	UInt mipmapCount = 1;
	Texture2D::FilterType filter = Texture2D::FT_LINEAR_LINEAR;
	Texture2D::WrapType warp = Texture2D::WT_CLAMP;
	UInt anisoLevel = 0;
	Buffer::UsageType usage = GetUsageType(pXmlNode);

	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (Is("Mipmaps", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%d", &mipmapCount);
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);	
		}
		else if (Is("FilterMode", attr->name()))
		{
			if (Is("Point", attr->value()))
			{
				filter = Texture2D::FT_NEAREST_NEAREST;
			}
			else if (Is("Bilinear", attr->value()))
			{
				filter = Texture2D::FT_LINEAR_NEAREST;
			}
			else if (Is("Trilinear", attr->value()))
			{
				filter = Texture2D::FT_LINEAR_LINEAR;
			}
		}
		else if (Is("AnisoLevel", attr->name()))
		{
			Int n;
			n = sscanf(attr->value(), "%d", &anisoLevel);
			WIRE_ASSERT_NO_SIDEEFFECTS(n == 1);	
		}
		else if (Is("WrapMode", attr->name()))
		{
			if (Is("Repeat", attr->value()))
			{
				warp = Texture2D::WT_REPEAT;
			}
			else if (Is("Clamp", attr->value()))
			{
				warp = Texture2D::WT_CLAMP;
			}
		}
		else if (Is("BlendMode", attr->name()))
		{
			if (Is("Replace", attr->value()))
			{
				blendMode = Material::BM_REPLACE;
			}
			else if (Is("Modulate", attr->value()))
			{
				blendMode = Material::BM_MODULATE;
			}
			else if (Is("Pass", attr->value()))
			{
				blendMode = Material::BM_PASS;
			}
			else if (Is("Blend", attr->value()))
			{
				blendMode = Material::BM_BLEND;
			}
			else if (Is("Decal", attr->value()))
			{
				blendMode = Material::BM_DECAL;
			}
		}
	}

	String path = String(mpPath) + String(pName);
	Image2D* pImage = LoadPNG(static_cast<const Char*>(path),
		(mipmapCount > 1), usage);
	if (!pImage)
	{
		return NULL;
	}

	Texture2D* pTexture = WIRE_NEW Texture2D(pImage, usage);
	mStatistics.TextureCount++;
	if (mipmapCount == 1)
	{
		if (filter == Texture2D::FT_NEAREST_NEAREST)
		{
			filter = Texture2D::FT_NEAREST;
		}
		else
		{
			filter = Texture2D::FT_LINEAR;
		}
	}

	pTexture->SetFilterType(filter);
	pTexture->SetWrapType(0, warp);
	pTexture->SetWrapType(1, warp);
	pTexture->SetAnisotropyValue(static_cast<Float>(anisoLevel));

	mTextures.Insert(pName, pTexture);

	return pTexture;
}

//----------------------------------------------------------------------------
void Importer::InitializeStaticSpatials(TArray<SpatialPtr>& rSpatials,
	Bool prepareSceneForStaticBatching,
	Bool duplicateSharedMeshesWhenPreparingSceneForStaticBatching)
{
	for (UInt i = 0; i < rSpatials.GetQuantity(); i++)
	{
		WIRE_ASSERT(rSpatials[i]);
		rSpatials[i]->WorldIsCurrent = true;
		rSpatials[i]->WorldBoundIsCurrent = true;
		if (prepareSceneForStaticBatching)
		{
			Geometry* pGeo = DynamicCast<Geometry>(rSpatials[i]);
			if (pGeo)
			{
				pGeo->MakeStatic(true,
					duplicateSharedMeshesWhenPreparingSceneForStaticBatching);
			}
		}
	}
}

//----------------------------------------------------------------------------
void Importer::RegisterColliders(TArray<Collider*>& rColliders,
	btDynamicsWorld* pPhysicsWorld)
{
	for (UInt i = 0; i < rColliders.GetQuantity(); i++)
	{
		WIRE_ASSERT(rColliders[i]);

		rColliders[i]->Register(pPhysicsWorld);
	}
}

//----------------------------------------------------------------------------
Bool Importer::Is(const Char* pSrc, const Char* pDst)
{
	return (System::Strcmp(pSrc, pDst) == 0);
}

//----------------------------------------------------------------------------
VertexBuffer* Importer::LoadVertexBufferFromFiles(Char* pFileName, Bool
	isVertexBufferBigEndian, Buffer::UsageType vertexBufferUsage, 
	Char* pNormalsName, Bool isNormalsBigEndian, Char* pColorsName, Bool
	isColorsBigEndian, TArray<Char*>& rUvSetNames, TArray<Bool>& rUvBigEndian)
{
	VertexAttributes vertexAttributes;
	vertexAttributes.SetPositionChannels(3);
	Int verticesSize;
	Float* pVertices = Load32(pFileName, verticesSize, isVertexBufferBigEndian);

	Int normalsSize;
	Float* pNormals = NULL;
	if (pNormalsName)
	{
		vertexAttributes.SetNormalChannels(3);
		pNormals = Load32(pNormalsName, normalsSize, isNormalsBigEndian);
		if (verticesSize != normalsSize)
		{
			WIRE_ASSERT(false /* Vertices and normals do not match */);
			Free32(pVertices);
			return NULL;
		}
	}

	Int colorsSize;
	Float* pColors = NULL;
	if (pColorsName)
	{
		vertexAttributes.SetColorChannels(4);
		pColors = Load32(pColorsName, colorsSize, isColorsBigEndian);
		if (verticesSize / (3 * sizeof(Float)) != colorsSize / (4 * sizeof(Float)))
		{
			WIRE_ASSERT(false /* Vertices and colors do not match */);
			Free32(pNormals);
			Free32(pVertices);
			return NULL;
		}
	}

	TArray<Float*> uvSets(rUvSetNames.GetQuantity());
	TArray<Int> uvSetSizes(rUvSetNames.GetQuantity());
	for (UInt i = 0; i < rUvSetNames.GetQuantity(); i++)
	{
		uvSetSizes.Append(0);
		uvSets.Append(Load32(rUvSetNames[i], uvSetSizes[i], rUvBigEndian[i]));
		vertexAttributes.SetTCoordChannels(2, i);
		if (verticesSize / (3 * sizeof(Float)) != uvSetSizes[i] / (2 * sizeof(Float)))
		{
			WIRE_ASSERT(false /* vertices and uv sets do not match */);
			for (UInt j = 0; j <= i; j++)
			{
				Free32(uvSets[j]);
			}

			Free32(pColors);
			Free32(pNormals);
			Free32(pVertices);
			return NULL;
		}	
	}

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(vertexAttributes,
		verticesSize / (3 * sizeof(Float)), vertexBufferUsage);
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

	return pVertexBuffer;
}

//----------------------------------------------------------------------------
IndexBuffer* Importer::LoadIndexBufferFromFile(Char* pFileName, Bool
	isIndexBufferBigEndian, Buffer::UsageType indexBufferUsage, Bool is16Bit)
{
	Int indicesSize;

	if (is16Bit)
	{
		UShort* pIndices = Load16(pFileName, indicesSize,
			isIndexBufferBigEndian);
		if (!pIndices)
		{
			return NULL;
		}

		IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(pIndices,
			indicesSize/sizeof(UShort), indexBufferUsage);
		mStatistics.IndexBufferCount++;
		return pIndexBuffer;
	}

	UInt* pIndices = reinterpret_cast<UInt*>(Load32(pFileName, indicesSize,
		isIndexBufferBigEndian));
	if (!pIndices)
	{
		return NULL;
	}

	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(indicesSize/sizeof(UInt),
		indexBufferUsage);
	mStatistics.IndexBufferCount++;
	for (UInt i = 0; i < indicesSize/sizeof(UInt); i++)
	{
		WIRE_ASSERT(pIndices[i] < 65536);
		(*pIndexBuffer)[i] = static_cast<UShort>(pIndices[i]);
	}

	UChar* pUChar = reinterpret_cast<UChar*>(pIndices);
	WIRE_DELETE[] pUChar;

	return pIndexBuffer;
}
