#include "Sample1.h"

#include "PerlinNoise.h"	// we use Perlin noise to create a texture

using namespace Wire;

WIRE_APPLICATION(Sample1);

//----------------------------------------------------------------------------
Sample1::Sample1()
{
}

//----------------------------------------------------------------------------
Sample1::~Sample1()
{
}

//----------------------------------------------------------------------------
Bool Sample1::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	// The smart pointer automatically deletes the object when it goes out
	// of scope and no other references to the object exist.
	mspCube = CreateCube();
	mspPyramid = CreatePyramid();

	// setup our camera at the origin
	// looking down the -z axis with y up
	Vector3F cameraLocation(0.0F, 0.0F, 0.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	Float width = static_cast<Float>(mpRenderer->GetWidth());
	Float height = static_cast<Float>(mpRenderer->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);

	mCuller.SetCamera(mspCamera);

	mAngle = 0.0F;

	mLastTime = System::GetTime();

	return true;
}

//----------------------------------------------------------------------------
Geometry* Sample1::CreateCube()
{
	Float extent = 1.0F;
	const Vector3F vertices[] = {
		Vector3F(-extent, -extent, -extent),
		Vector3F(+extent, -extent, -extent),
		Vector3F(+extent, +extent, -extent),
		Vector3F(-extent, +extent, -extent),
		Vector3F(-extent, -extent, +extent),
		Vector3F(+extent, -extent, +extent),
		Vector3F(+extent, +extent, +extent),
		Vector3F(-extent, +extent, +extent)
	};

	const ColorRGB colors[] = {
		ColorRGB(1.0F, 0.0F, 0.0F),
		ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F),
		ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F),
		ColorRGB(0.0F, 0.0F, 0.0F),
	};

	const Vector2F uvs[] = {
		Vector2F(0, 0),
		Vector2F(extent, 0),
		Vector2F(extent, extent),
		Vector2F(0, extent),
		Vector2F(0, 0),
		Vector2F(extent, 0),
		Vector2F(extent, extent),
		Vector2F(0, extent)
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetColorChannels(3);		// channels: R, G, B
	attributes.SetTCoordChannels(2);	// channels: U, V
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		pCubeVerts->Color3(i) = colors[i];
		pCubeVerts->TCoord2(i) = uvs[i];
	}

	UInt indices[] = {
		0, 2, 1,
		0, 3, 2,
		0, 1, 5,
		0, 5, 4,
		0, 4, 7,
		0, 7, 3,
		6, 4, 5,
		6, 7, 4,
		6, 5, 1,
		6, 1, 2,
		6, 2, 3,
		6, 3, 7,
	};

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for (UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

	Geometry* pCube = WIRE_NEW TriMesh(pCubeVerts, pIndices);
	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	Texture* pTexture = CreateTexture();
	pTextureEffect->Textures.Append(pTexture);
	pCube->AttachEffect(pTextureEffect);

	return pCube;
}

//----------------------------------------------------------------------------
Texture* Sample1::CreateTexture()
{
	const UInt width = 256;
	const UInt height = 64;

	Image::FormatMode format = Image::FM_RGB565;
	const UInt bytesPerPixel = Image::GetBytesPerPixel(format);

	UChar* pData = WIRE_NEW UChar[width * height * bytesPerPixel];
	UChar* pDst = reinterpret_cast<UChar*>(pData);

	PerlinNoise noise(0.5, 8);

	for (UInt y = 0; y < height; y++)
	{
		UInt temp;
		for (UInt x = 0; x < width; x++)
		{
			Float val = noise.EvaluateAtPos(
				static_cast<Float>(x)/static_cast<Float>(width),
				static_cast<Float>(y)/static_cast<Float>(height));
 			val = MathF::Cos(10 * val);
 			UChar t = static_cast<UChar>(val*127 + 127);

			UChar* pTemp = reinterpret_cast<UChar*>(&temp);
			*pTemp++ = t;
			*pTemp++ = t;
			*pTemp++ = ~t;
			*pTemp = 0xFF;

// 			Image::RGBA8888ToRGBA4444(pTemp - 3, pDst);
 			Image::RGB888ToRGB565(pTemp - 3, pDst);
 			pDst += 2;
		}
	}

	Image* pImage = WIRE_NEW Image(format, width, height, pData, false);
	Texture* pTexture = WIRE_NEW Texture(pImage);
	return pTexture;
}

//----------------------------------------------------------------------------
Geometry* Sample1::CreatePyramid()
{
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(3);
	VertexBuffer* pPyramidVerts = WIRE_NEW VertexBuffer(attributes, 12);

	ColorRGB colors[] = {
		ColorRGB(1.0F,0.0F,0.0F),			// Set The Color To Red
		ColorRGB(0.0F,1.0F,0.0F),			// Set The Color To Green
		ColorRGB(0.0F,0.0F,1.0F),			// Set The Color To Blue

		ColorRGB(1.0F,0.0F,0.0F),			// Set The Color To Red
		ColorRGB(0.0F,0.0F,1.0F),			// Set The Color To Blue
		ColorRGB(0.0F,1.0F,0.0F),			// Set The Color To Green

		ColorRGB(1.0F,0.0F,0.0F),			// Set The Color To Red
		ColorRGB(0.0F,0.0F,1.0F),			// Set The Color To Blue
		ColorRGB(0.0F,1.0F,0.0F),			// Set The Color To Green

		ColorRGB(1.0F,0.0F,0.0F),			// Set The Color To Red
		ColorRGB(0.0F,0.0F,1.0F),			// Set The Color To Blue
		ColorRGB(0.0F,1.0F,0.0F)			// Set The Color To Green
	};

	Vector3F vertices[] = {
		Vector3F( 0.0F, 1.0F, 0.0F),	// Top of Triangle (front)
		Vector3F(-1.0F,-1.0F, 1.0F),	// Left of Triangle (front)
		Vector3F( 1.0F,-1.0F, 1.0F),	// Right of Triangle (front)

		Vector3F( 0.0F, 1.0F, 0.0F),		// Top of Triangle (Right)
		Vector3F( 1.0F,-1.0F, 1.0F),	// Left of Triangle (Right)
		Vector3F( 1.0F,-1.0F,-1.0F),	// Right of Triangle (Right)

		Vector3F( 0.0F, 1.0F, 0.0F),		// Top of Triangle (Back)
		Vector3F(-1.0F,-1.0F,-1.0F),	// Left of Triangle (Back)
		Vector3F( 1.0F,-1.0F,-1.0F),	// Right of Triangle (Back)

		Vector3F( 0.0F, 1.0F, 0.0F),	// Top of Triangle (Left)
		Vector3F(-1.0F,-1.0F,-1.0F),	// Left of Triangle (Left)
		Vector3F(-1.0F,-1.0F, 1.0F)	// Right of Triangle (Left)
	};

	for (UInt i = 0; i < pPyramidVerts->GetVertexQuantity(); i++)
	{
		pPyramidVerts->Position3(i) = vertices[i];
		pPyramidVerts->Color3(i) = colors[i];
	}

	UInt indexQuantity = pPyramidVerts->GetVertexQuantity();
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for (UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = i;
	}

	Geometry* pPyramid = WIRE_NEW TriMesh(pPyramidVerts, pIndices);
	return pPyramid;
}

//----------------------------------------------------------------------------
void Sample1::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mCuller.SetFrustum(mspCamera->GetFrustum());

// 	Float scaleFactor = MathF::Sin(mAngle * 2.0F) * 0.8F;
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	mpRenderer->ClearBuffers();
	mpRenderer->PreDraw(mspCamera);

	Float angle = MathF::FMod(mAngle * 2.0F, MathF::TWO_PI);
	Float xOffset = -1.5F - angle;
	Matrix34F model(Vector3F(0, -1, 0), angle);
	mspPyramid->World.SetRotate(model);
	mspPyramid->World.SetTranslate(Vector3F(xOffset, 0.0F, -6.0F));
	mspPyramid->ModelBound->TransformBy(mspPyramid->World, mspPyramid->WorldBound);
//	mspPyramid->UpdateWorldBound();	// is the same but protected
	if (!mCuller.IsVisible(mspPyramid->WorldBound))
	{
// 		mspCube->VBuffer->Color3(0) = ColorRGB::WHITE;
// 		mspCube->VBuffer->Color3(1) = ColorRGB::WHITE;
// 		mspCube->VBuffer->Color3(2) = ColorRGB::WHITE;
//		mspCube->VBuffer->Release();
	}
	else
	{
// 		mspCube->VBuffer->Color3(0) = ColorRGB::RED;
// 		mspCube->VBuffer->Color3(1) = ColorRGB::GREEN;
// 		mspCube->VBuffer->Color3(2) = ColorRGB::BLUE;
//		mspCube->VBuffer->Release();
		mpRenderer->Draw(mspPyramid);
	}

	model.FromAxisAngle(Vector3F(1, 1, 1), mAngle);
	mspCube->World.SetRotate(model);
	mspCube->World.SetTranslate(Vector3F(1.5F, 0.0F, -7.0F));
//	mspCube->World.SetScale(Vector3F(scaleFactor + 1.0F, 1.0F, 1.0F));
//	mspCube->World.SetUniformScale(2.0F);
	mpRenderer->Draw(mspCube);

	mpRenderer->PostDraw();
	mpRenderer->DisplayBackBuffer();
}
