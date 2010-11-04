#include "Sample5.h"

using namespace Wire;

WIRE_APPLICATION(Sample5);

//----------------------------------------------------------------------------
Bool Sample5::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspRoot = WIRE_NEW Node;
	Node* pLitGroup = WIRE_NEW Node;
	Node* pUnlitGroup = WIRE_NEW Node;

	mspRoot->AttachChild(pLitGroup);
	mspRoot->AttachChild(pUnlitGroup);

	Spatial* pLightSrc1 = CreateCube(false, false, true, ColorRGBA(0, 1, 0, 0));
	pLightSrc1->Local.SetUniformScale(0.2F);
	pUnlitGroup->AttachChild(pLightSrc1);

	Spatial* pCube1 = CreateCube();
	Spatial* pCube2 = CreateCube(false, true);
	pCube1->Local.SetTranslate(Vector3F(-2.5F, 0, 0));
	pCube2->Local.SetTranslate(Vector3F(2.5F, 0, 0));
	pLitGroup->AttachChild(pCube1);
	pLitGroup->AttachChild(pCube2);

	MaterialState* pMaterialState = WIRE_NEW MaterialState;
	pMaterialState->Ambient = ColorRGBA(1, 1, 0, 1);

	Light* pLight1 = WIRE_NEW Light(Light::LT_POINT);
	pLight1->Position = Vector3F(0, 0, 0);
	pLight1->Color = ColorRGB(0, 1, 0);

	Light* pLight2 = WIRE_NEW Light();
	pLight2->Color = ColorRGB(1, 0.3F, 0);

	pLitGroup->AttachGlobalState(pMaterialState);
	pLitGroup->AttachLight(pLight1);
	pLitGroup->AttachLight(pLight2);
	mspRoot->UpdateRS();


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
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);
	mCuller.SetCamera(mspCamera);

	// Initialize working variables used in the render loop (i.e. OnIdle()).
	mAngle = 0.0F;
	mLastTime = System::GetTime();

	return true;
}

//----------------------------------------------------------------------------
void Sample5::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI * 2);

	Node* pLitGroup = DynamicCast<Node>(mspRoot->GetChild(0));
	WIRE_ASSERT(pLitGroup);

	Matrix34F model1(Vector3F(0.75F, 0.25F, 0.5F), -mAngle * 0.5F);
	Spatial* pCube1 = pLitGroup->GetChild(0);
	pCube1->Local.SetRotate(model1);

	Matrix34F model2(Vector3F(-0.75F, -0.25F, -0.5F), -mAngle * 0.5F);
	Spatial* pCube2 = pLitGroup->GetChild(1);
	pCube2->Local.SetRotate(model2);

	Float y = MathF::FMod(static_cast<Float>(time), MathF::TWO_PI);
	Vector3F lightPos(0, MathF::Sin(y*2) * 2, 0);
	pLitGroup->GetLight()->Position = lightPos;

	Node* pUnlitGroup = DynamicCast<Node>(mspRoot->GetChild(1));
	WIRE_ASSERT(pUnlitGroup);
	Spatial* pLightCube1 = pUnlitGroup->GetChild(0);
	pLightCube1->Local.SetTranslate(lightPos);

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSet());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample5::CreateCube(Bool useTexture, Bool useNormals,
	Bool useVertexColor, ColorRGBA vertexColor)
{
	// Create a cube with unique texture (UV) coordinates for each side.
	// This means we have to duplicate vertices, since every vertex can only
	// have one UV coordinate per texture. Thus resulting in 24 (4 vertices
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
	// It consists of 3d positions and 2d texture coordinates
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z

	if (useTexture)
	{
		attributes.SetTCoordChannels(2);	// channels: U, V
	}

	if (useNormals)
	{
		attributes.SetNormalChannels(3);	// channels: X, Y, Z
	}

	if (useVertexColor)
	{
		attributes.SetColorChannels(4);		// RGBA
	}

	// Now with the attributes being defined, we can create a VertexBuffer
	// and fill it with data.
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];

		if (useTexture)
		{
			pCubeVerts->TCoord2(i) = uvs[i];
		}

		if (useVertexColor)
		{
			pCubeVerts->Color4(i) = vertexColor;
		}
	}

	// Same for the IndexBuffer
	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

	// Geometric objects consist of a Vertex- and IndexBuffer.
	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
	pCube->GenerateNormals();

	if (useTexture)
	{
		TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
		Texture2D* pTexture = CreateTexture();
		pTextureEffect->Textures.Append(pTexture);
		pTextureEffect->BlendOps.Append(TextureEffect::BM_MODULATE);
		pCube->AttachEffect(pTextureEffect);
	}

	return pCube;
}

//----------------------------------------------------------------------------
struct Cell
{
	Vector2F point;
	ColorRGB color;
};

//----------------------------------------------------------------------------
Texture2D* Sample5::CreateTexture()
{
	if (mspTexture)
	{
		return mspTexture;
	}

	// define the properties of the image to be used as a texture
	const UInt width = 256;
	const UInt height = 256;
	const Image2D::FormatMode format = Image2D::FM_RGB888;
	const UInt bpp = Image2D::GetBytesPerPixel(format);
	ColorRGB* const pColorDst = WIRE_NEW ColorRGB[width*height];

	// create points with random x,y position and color
	TArray<Cell> cells;
	Random random;
	for (UInt i = 0; i < 10; i++)
	{
		Cell cell;
		cell.point.X() = random.GetFloat() * width;
		cell.point.Y() = random.GetFloat() * height;
		cell.color.R() = random.GetFloat();
		cell.color.G() = random.GetFloat();
		cell.color.B() = random.GetFloat();

		Float max = 0.0F;
		max = max < cell.color.R() ? cell.color.R() : max;
		max = max < cell.color.G() ? cell.color.G() : max;
		max = max < cell.color.B() ? cell.color.B() : max;
		max = 1.0F / max;
		cell.color *= max;
		cells.Append(cell);
	}

	// iterate over all texels and use the distance to the 2 closest random
	// points to calculate the texel's color
	Float max = 0;
	for (UInt y = 0; y < height; y++)
	{
		for (UInt x = 0; x < width; x++)
		{
			Float minDist = MathF::MAX_REAL;
			Float min2Dist = MathF::MAX_REAL;
			UInt minIndex = 0;

			for (UInt i = 0; i < cells.GetQuantity(); i++)
			{
				Vector2F pos(static_cast<Float>(x), static_cast<Float>(y));

				// Handle tiling
				Vector2F vec = cells[i].point - pos;
				vec.X() = MathF::FAbs(vec.X());
				vec.Y() = MathF::FAbs(vec.Y());
				vec.X() = vec.X() > width/2 ? width-vec.X() : vec.X();
				vec.Y() = vec.Y() > height/2 ? height-vec.Y() : vec.Y();

				Float distance = vec.Length();

				if (minDist > distance)
				{
					min2Dist = minDist;
					minDist = distance;
					minIndex = i;
				}
				else if (min2Dist > distance)
				{
					min2Dist = distance;
				}
			}

			float factor = (min2Dist - minDist) + 3;
			ColorRGB color = cells[minIndex].color * factor;
			max = max < color.R() ? color.R() : max;
			max = max < color.G() ? color.G() : max;
			max = max < color.B() ? color.B() : max;
			pColorDst[y*width+x] = color;
		}
	}

	// convert and normalize the ColorRGBA float array to an 8-bit per
	// channel texture
	max = 255.0F / max;
	UChar* const pDst = WIRE_NEW UChar[width * height * bpp];
	for (UInt i = 0; i < width*height; i++)
	{
		ColorRGB color = pColorDst[i];
		pDst[i*bpp] = static_cast<UChar>(color.R() * max);
		pDst[i*bpp+1] = static_cast<UChar>(color.G() * max);
		pDst[i*bpp+2] = static_cast<UChar>(color.B() * max);
	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	// The texture tiles and the torus is supposed to be seamless, therefore
	// we need the UV set to be repeating.
	pTexture->SetWrapType(0, Texture2D::WT_REPEAT);
	pTexture->SetWrapType(1, Texture2D::WT_REPEAT);

	mspTexture = pTexture;
	return pTexture;
}
