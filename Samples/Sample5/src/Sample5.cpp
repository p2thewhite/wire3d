// Sample5 - Lighting
// This sample demonstrates how to render objects using lights.
// For that purpose we create 2 cubes which are both lit by 2 point lights
// using the scene graph. Furthermore we create a plane which is lit by a 
// spot light and rendered without using the scene graph.
// The positions of the lights are represent by small cubes which are colored
// using the light's color.

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

	// We are building the following scenegraph:
	// +Root
	// +->LitGroup (with StateMaterial, Light1, Light2 attached)
	// | +->Cube1 (textured cube)
	// | +->Cube2 (flat shaded cube)
	// |
	// +->LightNode1 (owning Light1)
	// | +->LightSrc1 (cube representing Light1)
	// |
	// +->LightNode2 (owning Light2)
	//   +->LightSrc2 (cube representing Light2)

	mspRoot = WIRE_NEW Node;
	Node* pLitGroup = WIRE_NEW Node;
	mspRoot->AttachChild(pLitGroup);

	// create a green and a red point light and a material state
	// which defines how to reflect the lights.
	Light* pLight1 = WIRE_NEW Light(Light::LT_POINT);
	pLight1->Color = ColorRGB::GREEN;

	Light* pLight2 = WIRE_NEW Light(Light::LT_POINT);
	pLight2->Color = ColorRGB::RED;
	pLight2->Ambient = ColorRGB(0.1F, 0.1F, 0.1F);

	StateMaterial* pMaterialState = WIRE_NEW StateMaterial;
	pMaterialState->Ambient = ColorRGBA(1, 1, 0, 1);

	// attach lights and material state
	pLitGroup->AttachGlobalState(pMaterialState);
	pLitGroup->AttachLight(pLight1);
	pLitGroup->AttachLight(pLight2);

	// create and position the actual objects to be lit
	Spatial* pCube1 = CreateCube();
	Spatial* pCube2 = CreateCube(false, true);
	pCube1->Local.SetTranslate(Vector3F(-2.0F, 0, 0));
	pCube2->Local.SetTranslate(Vector3F(2.5F, 0, 0));
	pLitGroup->AttachChild(pCube1);
	pLitGroup->AttachChild(pCube2);

	// create light nodes. These act the same as normal nodes, except that
	// the own a light and apply scene graph transformations on that light.
	NodeLight* pLightNode1 = WIRE_NEW NodeLight;
	mspRoot->AttachChild(pLightNode1);
	pLightNode1->SetLight(pLight1);

	NodeLight* pLightNode2 = WIRE_NEW NodeLight;
	mspRoot->AttachChild(pLightNode2);
	pLightNode2->SetLight(pLight2);

	// create two cubes representing the position and color of the lights
	Spatial* pLightSrc1 = CreateCube(false, false, true, ColorRGBA::GREEN);
	pLightSrc1->Local.SetUniformScale(0.15F);
	pLightNode1->AttachChild(pLightSrc1);

	Spatial* pLightSrc2 = CreateCube(false, false, true, ColorRGBA::RED);
	pLightSrc2->Local.SetUniformScale(0.15F);
	pLightNode2->AttachChild(pLightSrc2);

	// update all render state and light objects in the scene graph
	// (i.e. Light1, Light2 and StateMaterial are propagated from LitGroup
	// to Cube1 and Cube2)
	mspRoot->UpdateRS();

	// create the bottom plane, which already has a white light attached,
	// to be rendered manually in the render loop. This is only for the
	// purpose of demonstrating scene graph and manual rendering with lights.
	mspPlane = CreatePlane();
	mspWhiteCube = CreateCube(false, false, true, ColorRGBA::WHITE);
	mspWhiteCube->World.SetUniformScale(0.15F);

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, -1.0F, 10.0F);
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

	// scene graph transformations
	//
	Node* pLitGroup = DynamicCast<Node>(mspRoot->GetChild(0));
	WIRE_ASSERT(pLitGroup);

	// rotate the 2 cubes
	Matrix34F rotate1(Vector3F(0.75F, 0.25F, 0.5F), -mAngle * 0.5F);
	Spatial* pCube1 = pLitGroup->GetChild(0);
	pCube1->Local.SetRotate(rotate1);

	Matrix34F rotate2(Vector3F(-0.75F, -0.25F, -0.5F), -mAngle * 0.5F);
	Spatial* pCube2 = pLitGroup->GetChild(1);
	pCube2->Local.SetRotate(rotate2);

	// move the green light up and down
	Float y = MathF::FMod(static_cast<Float>(time), MathF::TWO_PI);
	Vector3F lightPos1(0, MathF::Sin(y*2) * 1.5F, 2);
	Node* pLightNode1 = DynamicCast<Node>(mspRoot->GetChild(1));
	WIRE_ASSERT(pLightNode1);
	pLightNode1->Local.SetTranslate(lightPos1);

	// rotate the red light about the y axis
	Node* pLightNode2 = DynamicCast<Node>(mspRoot->GetChild(2));
	WIRE_ASSERT(pLightNode2);
	Matrix34F rotateLight2(Vector3F::UNIT_Y, mAngle);
	Vector3F lightPos2 = Vector3F(5, 0, 0) * rotateLight2;
	pLightNode2->Local.SetTranslate(lightPos2);

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	// manual transformation from local to world space of 
	// the non-scene graph part
	//
	Float angle = MathF::Sin(mAngle*2);
	angle = angle * MathF::HALF_PI*0.3F + MathF::PI;
	Matrix34F rotateLocalLight3(Vector3F(0, 1, 0), angle);
	Matrix34F rotateWorldLight3(Vector3F(1, 0, 0), -0.5F);
	rotateWorldLight3 = rotateWorldLight3 * rotateLocalLight3;
	mspWhiteCube->World.SetTranslate(
		Vector3F(0.5F, -1.0F, 4 + MathF::Sin(y * 1.0F) * 2));
	mspWhiteCube->World.SetRotate(rotateWorldLight3);
	mspPlane->Lights[0]->Position = mspWhiteCube->World.GetTranslate();
	mspPlane->Lights[0]->Direction = mspWhiteCube->World.GetMatrix().
		GetColumn(2);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);

	// render the scene graph
	GetRenderer()->DrawScene(mCuller.GetVisibleSet());

	// render the white cube representing the spot light
	if (mCuller.IsVisible(mspWhiteCube))
	{
		GetRenderer()->Draw(mspWhiteCube);
	}

	// render the bottom plane which is being lit by the spot light
	GetRenderer()->EnableLighting();
	if (mCuller.IsVisible(mspPlane))
	{
		GetRenderer()->Draw(mspPlane);
	}

	GetRenderer()->DisableLighting();

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample5::CreateCube(Bool useTexture, Bool useNormals,
	Bool useVertexColor, ColorRGBA vertexColor)
{
	// Creation of Wire::Geometry objects is explained in detail in Sample1

	const Vector3F vertices[] =
	{
		Vector3F(-1.0F,  1.0F,  1.0F), Vector3F( 1.0F,  1.0F,  1.0F),
		Vector3F( 1.0F, -1.0F,  1.0F), Vector3F(-1.0F, -1.0F,  1.0F),
		Vector3F( 1.0F,  1.0F, -1.0F), Vector3F( 1.0F,  1.0F,  1.0F),
		Vector3F(-1.0F,  1.0F,  1.0F), Vector3F(-1.0F,  1.0F, -1.0F),
		Vector3F( 1.0F, -1.0F,  1.0F), Vector3F( 1.0F,  1.0F,  1.0F),
		Vector3F( 1.0F,  1.0F, -1.0F), Vector3F( 1.0F, -1.0F, -1.0F),
		Vector3F(-1.0F,  1.0F, -1.0F), Vector3F( 1.0F,  1.0F, -1.0F),
		Vector3F( 1.0F, -1.0F, -1.0F), Vector3F(-1.0F, -1.0F, -1.0F),
		Vector3F( 1.0F, -1.0F, -1.0F), Vector3F( 1.0F, -1.0F,  1.0F),
		Vector3F(-1.0F, -1.0F,  1.0F), Vector3F(-1.0F, -1.0F, -1.0F),
		Vector3F(-1.0F, -1.0F,  1.0F), Vector3F(-1.0F,  1.0F,  1.0F),
		Vector3F(-1.0F,  1.0F, -1.0F), Vector3F(-1.0F, -1.0F, -1.0F)
	};

	const Vector2F uvs[] =
	{
		Vector2F(0.0F, 0.0F), Vector2F(1.0F, 0.0F), Vector2F(1.0F, 1.0F),
		Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F),	Vector2F(1.0F, 0.0F),
		Vector2F(1.0F, 1.0F), Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F),
		Vector2F(1.0F, 0.0F), Vector2F(1.0F, 1.0F),	Vector2F(0.0F, 1.0F),
		Vector2F(0.0F, 0.0F), Vector2F(1.0F, 0.0F),	Vector2F(1.0F, 1.0F),
		Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F), Vector2F(1.0F, 0.0F),
		Vector2F(1.0F, 1.0F), Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F),
		Vector2F(1.0F, 0.0F), Vector2F(1.0F, 1.0F), Vector2F(0.0F, 1.0F)
	};

	const UInt indices[] =
	{
		0, 2, 1, 0, 3, 2, 4, 6, 5, 4, 7, 6,	8, 10, 9, 8, 11, 10, 12, 13, 14,
		12, 14, 15, 16, 17, 18,	16, 18, 19,	20, 21, 22,	20, 22, 23
	};

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
			if (vertices[i].Z() > 0)
			{
				pCubeVerts->Color4(i) = vertexColor;
			}
			else
			{
				pCubeVerts->Color4(i) = vertexColor * 0.5F;
			}
		}
	}

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

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
Geometry* Sample5::CreatePlane()
{
	const UInt tileXCount = 30;
	const UInt tileYCount = 20;
	const Float xSize = 12.0F;
	const Float ySize = 8.0F;

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetTCoordChannels(2);	// channels: U, V
	attributes.SetNormalChannels(3);	// channels: X, Y, Z

	const UInt vertexCount = (tileXCount+1) * (tileYCount+1);
	VertexBuffer* pVertices = WIRE_NEW VertexBuffer(attributes, vertexCount);

	const Float xStride = xSize / tileXCount;
	const Float yStride = ySize / tileYCount;
	Float y = -ySize * 0.5F;
	for (UInt j = 0; j < tileYCount+1; j++)
	{
		Float x = -xSize * 0.5F;;
		for (UInt i = 0; i < tileXCount+1; i++)
		{
			pVertices->Position3(i + (tileXCount+1)*j) = Vector3F(x, y, 0);
			pVertices->TCoord2(i + (tileXCount+1)*j) = Vector2F(x, y);
			x += xStride;
		}

		y += yStride;
	}

	const UInt indexCount = tileXCount * tileYCount * 6;
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexCount);

	for (UInt j = 0; j < tileYCount; j++)
	{
		UInt offset = (tileXCount+1)*j;
		for (UInt i = 0; i < tileXCount; i++)
		{
			UInt index = tileXCount*j+i;
			UInt index0 = i+offset;
			UInt index1 = index0+tileXCount+1;
			UInt index2 = index0+1;
			UInt index3 = index0+tileXCount+2;

			(*pIndices)[index*6] = index0;
			(*pIndices)[index*6+1] = index2;
			(*pIndices)[index*6+2] = index1;

			(*pIndices)[index*6+3] = index1;
			(*pIndices)[index*6+4] = index2;
			(*pIndices)[index*6+5] = index3;

		}
	}

	Geometry* pGeo = WIRE_NEW Geometry(pVertices, pIndices);
	pGeo->GenerateNormals();

	Matrix34F rotate(Vector3F(1.0F, 0, 0), -1.0F);
	pGeo->World.SetRotate(rotate);
	pGeo->World.SetTranslate(Vector3F(0, -2.5F, 0));

	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	Texture2D* pTexture = CreateTexture();
	pTexture->SetWrapType(0, Texture2D::WT_REPEAT);
	pTexture->SetWrapType(1, Texture2D::WT_REPEAT);
	pTextureEffect->Textures.Append(pTexture);
	pTextureEffect->BlendOps.Append(TextureEffect::BM_MODULATE);
	pGeo->AttachEffect(pTextureEffect);

	// attach a material state and a light to the plane geometry directly
	StateMaterial* pMaterial = WIRE_NEW StateMaterial;
	pMaterial->Ambient = ColorRGBA(1, 1, 1, 1);
	pGeo->States[State::MATERIAL] = pMaterial;

	Light* pLight = WIRE_NEW Light(Light::LT_SPOT);
	pLight->Position = Vector3F(0, 0, 10);
	pLight->Direction = Vector3F(0, 0, -1);
	pLight->Angle = 0.5F;
	pLight->Ambient = ColorRGB(0.2F, 0.2F, 0.2F);
	pGeo->Lights.Append(pLight);

	// Notice the difference between attaching a light directly:
	// 	pGeo->Lights.Append(pLight);
	//
	// and attaching a light via the AttachLight() method used
	// in OnInitialize():
	//	pGeo->AttachLight(pLight);
	//
	// pGeo->Lights[] holds all lights that will be used for rendering.
	// pGeo->AttachLight() adds a light to an internal container and
	// needs UpdateRS() to accumulate all lights from the internal container
	// and the lights of all the parents of pGeo into pGeo->Lights.
	// In short: use the first approach for direct rendering and the second
	// if you are using the scene graph for rendering.

	return pGeo;
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

			Float factor = (min2Dist - minDist) + 3;
			ColorRGB color = cells[minIndex].color * factor;
			pColorDst[y*width+x] = color;

			max = max < color.R() ? color.R() : max;
			max = max < color.G() ? color.G() : max;
			max = max < color.B() ? color.B() : max;
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
	// The texture tiles are supposed to be seamless, therefore
	// we need the UV set to be repeating.
	pTexture->SetWrapType(0, Texture2D::WT_REPEAT);
	pTexture->SetWrapType(1, Texture2D::WT_REPEAT);

	// save the texture for later reference
	mspTexture = pTexture;

	return pTexture;
}
