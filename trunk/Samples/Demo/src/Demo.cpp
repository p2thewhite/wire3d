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
//	LoadScene("Data/InnsbruckMobile.xml");

	mspCube = CreateCube();

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, 0.0F, 10.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	UInt width = GetRenderer()->GetWidth();
	UInt height = GetRenderer()->GetHeight();
	Float fieldOfView = 45.0F;
	Float aspectRatio = static_cast<Float>(width)/static_cast<Float>(height);
	Float nearPlane = 0.1F;
	Float farPlane = 300.0F;
	mspCamera->SetFrustum(fieldOfView, aspectRatio, nearPlane, farPlane);
	mCuller.SetCamera(mspCamera);

	// We render some of the cubes using transparency and front-face culling,
	// so we create the required render state objects here.
	mspCull = WIRE_NEW StateCull;
	mspAlpha = WIRE_NEW StateAlpha;

	// We render the top row of cubes with lighting on, so we create a light
	// and a material state for the cube, which defines how the light (color)
	// will be reflected.
	mspLight = WIRE_NEW Light;
	mspMaterial = WIRE_NEW StateMaterial;

	// Initialize working variables used in the render loop (i.e. OnIdle()).
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
	mAngle = 0;

	// If the camera's viewing frustum changed, we need to update the culler
	// (we know we don't change it, so it's commented out here)
//	mCuller.SetFrustum(mspCamera->GetFrustum());

	// Clear the framebuffer and the z-buffer.
	GetRenderer()->ClearBuffers();

	// Tell the Renderer that we want to start drawing.
	GetRenderer()->PreDraw(mspCamera);

	// We set the render state to backface culling and disable alpha blending.
	// NOTE: if you are not using the scenegraph to handle render states for
	// you, it is your responsibility to handle states between draw calls.

	// use back face culling
	mspCull->CullFace = StateCull::CM_BACK;
	GetRenderer()->SetState(mspCull);

	// disable alpha blending
	mspAlpha->BlendEnabled = false;
	GetRenderer()->SetState(mspAlpha);

	// The light's color is white. We set the material of the cube so it
	// reduces the red and blue component a bit, resulting in a tint of green.
 	mspMaterial->Ambient = ColorRGBA(0.9F, 1.0F, 0.9F, 1.0F);
 	GetRenderer()->SetState(mspMaterial);

	// modulate the texture with the light
	mspCube->GetMaterial()->SetBlendMode(Material::BM_MODULATE);

	GetRenderer()->EnableLighting(mspLight->Ambient);
	GetRenderer()->SetLight(mspLight);

	// Draw the upper row of cubes.
	const UInt cubeCount = 5;
	const Float stride = 3.5F;
	const Float offset = cubeCount * -0.5F * stride + stride * 0.6F;
	Float z = MathF::Sin(mAngle) * 3.0F;

	for (UInt i = 0; i < cubeCount; i++)
	{
		// Rotate the cube around the axis specified by the vector and
		// the angle given in radians.
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), -mAngle - 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, 1.8F, z));

		// After setting world transformation, we update the world bounding
		// volume of the cube so we can cull it against the viewing frustum.
		// That way we only draw objects that are visible on the screen.
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			GetRenderer()->Draw(mspCube);
		}
	}

	// Draw the lower row of cubes using alpha blending.
	// For correct transparency order, we draw the backfaces first and then
	// the transparent frontfaces.

	// To draw the backfaces, cull the frontfaces
	mspCull->CullFace = StateCull::CM_FRONT;
	GetRenderer()->SetState(mspCull);

	GetRenderer()->DisableLighting();

	// There is no more light, so the texture blending needs to be reset,
	// otherwise it will modulate undefined values.
	mspCube->GetMaterial()->SetBlendMode(Material::BM_REPLACE);
	
	z = MathF::Cos(mAngle) * 3.0F;
	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			GetRenderer()->Draw(mspCube);
		}
	}

	// Cull the backfaces, i.e. draw the front faces using alpha blending
	mspCull->CullFace = StateCull::CM_BACK;
	GetRenderer()->SetState(mspCull);
	mspAlpha->BlendEnabled = true;
	GetRenderer()->SetState(mspAlpha);

	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			GetRenderer()->Draw(mspCube);
		}
	}

	// Tell the Renderer that we are done with drawing
	GetRenderer()->PostDraw();

	// Present the framebuffer (aka backbuffer) on the screen
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Demo::CreateCube()
{
	// Create a cube with unique texture (UV) coordinates for each side.
	// This means we have to duplicate vertices, since every vertex can only
	// have one UV coordinate per texture. This results in 24 (4 vertices
	// times 6 sides of the cube) vertices.
	const Float extent = 1.0F;
	const Vector3F vertices[] =
	{
		// side 1
		Vector3F(-extent,  extent,  extent),
		Vector3F( extent,  extent,  extent),
		Vector3F( extent, -extent,  extent),
		Vector3F(-extent, -extent,  extent),
		// side 2
		Vector3F( extent,  extent, -extent),
		Vector3F( extent,  extent,  extent),
		Vector3F(-extent,  extent,  extent),
		Vector3F(-extent,  extent, -extent),
		// side 3
		Vector3F( extent, -extent,  extent),
		Vector3F( extent,  extent,  extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent, -extent, -extent),
		// side 4
		Vector3F(-extent,  extent, -extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent, -extent, -extent),
		Vector3F(-extent, -extent, -extent),
		// side 5
		Vector3F( extent, -extent, -extent),
		Vector3F( extent, -extent,  extent),
		Vector3F(-extent, -extent,  extent),
		Vector3F(-extent, -extent, -extent),
		// side 6
		Vector3F(-extent, -extent,  extent),
		Vector3F(-extent,  extent,  extent),
		Vector3F(-extent,  extent, -extent),
		Vector3F(-extent, -extent, -extent)
	};

	// Texture coordinates
	const Float extentUv = 1.0F;
	const Vector2F uvs[] =
	{
		// side 1
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 2
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 3
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 4
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 5
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 6
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv)
	};

	// Indices provide connectivity information and define the triangle mesh.
	// Every side of the cube consists of 2 triangles.
	const UInt indices[] =
	{
		// side 1
		0, 2, 1,
		0, 3, 2,
		// side 2
		4, 6, 5,
		4, 7, 6,
		// side 3
		8, 10, 9,
		8, 11, 10,
		// side 4
		12, 13, 14,
		12, 14, 15,
		// side 5
		16, 17, 18,
		16, 18, 19,
		// side 6
		20, 21, 22,
		20, 22, 23
	};

	// Before creating the VertexBuffer we need to define its format.
	// It consists of 3d positions, 2d texture coordinates and 3d normal
	// vectors. Normals are required for lighting.
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetTCoordChannels(2);	// channels: U, V
	attributes.SetNormalChannels(3);	// channels: X, Y, Z

	// Now with the attributes being defined, we can create a VertexBuffer
	// and fill it with data.
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);

	// If the supplied number of vertices does not match the number of uvs,
	// the assert fires and halts the application (in debug mode only)
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));

	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pVBuffer->GetQuantity(); i++)
	{
		pVBuffer->Position3(i) = vertices[i];
		pVBuffer->TCoord2(i) = uvs[i];

		// We don't provide normals here, we generate them later.
//		pVBuffer->Normal3(i) = normals[i];
	}

	// Fill the IndexBuffer with data.
	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIBuffer)[i] = indices[i];
	}

	// The cube shall be textured. Therefore we create and attach a material,
	// where we add textures and define their blending modes.
 	Material* pMaterial = WIRE_NEW Material;
 	pMaterial->AddTexture(CreateTexture(), Material::BM_MODULATE);

	// Geometric objects consist of a Vertex-, an IndexBuffer and optionally
	// a material
	Geometry* pCube = WIRE_NEW Geometry(pVBuffer, pIBuffer, pMaterial);

	// Generate normal vectors from the triangles of the geometry.
	pCube->GetMesh()->GenerateNormals();

	// NOTE: Geometry takes ownership over Vertex- and IndexBuffers using
	// smart pointers. Thus, you can share these buffers amongst Geometry 
	// objects without having to worry about deletion. Same applies to
	// Materials, Effects, Textures and Images.

	return pCube;
}

//----------------------------------------------------------------------------
Texture2D* Demo::CreateTexture()
{
	Image2D* pImage = LoadImage("Village.png", true);

	// Create a Texture using this image. Images can be shared amongst
	// texture objects. So you can use different filter/wrap/anisotropy values
	// without the need to duplicate images in memory.
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);

	// Use anisotropic filtering for texturing. Looks better, but lowers
	// performance.
	pTexture->SetAnisotropyValue(4.0F);

	return pTexture;
}




//----------------------------------------------------------------------------
Node* Demo::LoadScene(const Char* pFilename)
{
	Char* pXml;
	Int xmlSize;
	Bool hasSucceeded;
	hasSucceeded = System::Load(pFilename, pXml, xmlSize);
	WIRE_ASSERT(hasSucceeded /* Could not load file */);

	Char* pXmlNullTerminated = WIRE_NEW Char[xmlSize+1];
	System::Memcpy(pXmlNullTerminated, xmlSize, pXml, xmlSize);
	pXmlNullTerminated[xmlSize] = 0;

	rapidxml::xml_document<> doc;    // character type defaults to char
	doc.parse<0>(pXmlNullTerminated);
	mspRoot = Traverse(doc.first_node());

	return NULL;
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

	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		String str = String("XmlNode has attribute ") + String(attr->name()) +
			String(" with value ") + String(attr->value()) + String ("\n");
		System::Print(str);
	}

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
Node* Demo::ParseNode(rapidxml::xml_node<>* pXmlNode)
{
	Node* pNode = WIRE_NEW Node;
	Vector3F t;
	QuaternionF r;
	Vector3F s;

	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (System::Strcmp("Name", attr->name()) == 0)
		{
			continue;
		}
		else if (System::Strcmp("Pos", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f", &t.X(), &t.Y(), &t.Z());
			WIRE_ASSERT(n == 3);
		}
		else if (System::Strcmp("Rot", attr->name()) == 0)
		{
			// TODO
		}
		else if (System::Strcmp("Scale", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%f, %f, %f", &s.X(), &s.Y(), &s.Z());
			WIRE_ASSERT(n == 3);
		}
		else
		{
			WIRE_ASSERT(false);
		}
	}

	
	if ((t.X() != 0) || (t.Y() != 0) || (t.Z() != 0))
	{
		pNode->Local.SetTranslate(t);
	}

	if ((s.X() != 1.0f) || (s.Y() != 1.0f) || (s.Z() != 1.0f))
	{
		if ((s.X() == s.Y()) && (s.X() == s.Z()))
		{
			pNode->Local.SetUniformScale(s.X());
		}
		else
		{
			pNode->Local.SetScale(s);
		}
	}

	return pNode;
}

//----------------------------------------------------------------------------
Geometry* Demo::ParseLeaf(rapidxml::xml_node<>* pXmlNode)
{
	Mesh* pMesh;
	Material* pMaterial;

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

	Geometry* pGeo = WIRE_NEW Geometry(pMesh, pMaterial);

	// TODO: trafo

	return NULL;
}

//----------------------------------------------------------------------------
Mesh* Demo::ParseMesh(rapidxml::xml_node<>* pXmlNode)
{
	return NULL;
}

//----------------------------------------------------------------------------
Material* Demo::ParseMaterial(rapidxml::xml_node<>* pXmlNode)
{
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

	return pMaterial;
}

//----------------------------------------------------------------------------
Texture2D* Demo::ParseTexture(rapidxml::xml_node<>* pXmlNode)
{
	Char* pName = NULL;
	UInt mipmapCount = 1;
	Texture2D::FilterType filter = Texture2D::FT_LINEAR_LINEAR;
	Texture2D::WrapType warp = Texture2D::WT_CLAMP;
	UInt anisoLevel = 0;

	// TODO: remove width, height, format from xml

	for (rapidxml::xml_attribute<>* attr = pXmlNode->first_attribute();	attr;
		attr = attr->next_attribute())
	{
		if (System::Strcmp("Name", attr->name()) == 0)
		{
			pName = attr->value();
		}
		else if (System::Strcmp("Mipmaps", attr->name()) == 0)
		{
			Int n;
			n = sscanf(attr->value(), "%d", &mipmapCount);
			WIRE_ASSERT(n == 1);	
		}
		else if (System::Strcmp("FilterMode", attr->name()) == 0)
		{
			// TODO check if names are correct
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
			// TODO check if names are correct
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
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetFilterType(filter);
	pTexture->SetWrapType(0, warp);
	pTexture->SetWrapType(1, warp);
	pTexture->SetAnisotropyValue(static_cast<Float>(anisoLevel));

	return pTexture;
}

//----------------------------------------------------------------------------
Image2D* Demo::LoadImage(const Char* pFilename, Bool hasMipmaps)
{
	String path = String(mpPath) + String(pFilename);
	UChar* pPNG;
	Int pngSize;
	Bool hasSucceeded = System::Load(static_cast<const Char*>(path), pPNG,
		pngSize);
	if (!hasSucceeded)
	{
		WIRE_ASSERT(false /* Could not load file */);
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
