#include "Sample1.h"

#include "PerlinNoise.h"	// we use Perlin noise to create a texture

using namespace Wire;

WIRE_APPLICATION(Sample1);

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

	// setup our camera to look down the -z axis with y up
	Vector3F cameraLocation(0.0F, 0.0F, 15.0F);
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

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	Texture2D* pTexture = CreateTexture();
	pTextureEffect->Textures.Append(pTexture);
	pTextureEffect->BlendOps.Append(TextureEffect::BM_MODULATE);
	pCube->AttachEffect(pTextureEffect);

	return pCube;
}

//----------------------------------------------------------------------------
Texture2D* Sample1::CreateTexture()
{
	const UInt width = 256;
	const UInt height = 64;

	Image2D::FormatMode format = Image2D::FM_RGB565;
	const UInt bytesPerPixel = Image2D::GetBytesPerPixel(format);

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

// 			Image2D::RGBA8888ToRGBA4444(pTemp - 3, pDst);
 			Image2D::RGB888ToRGB565(pTemp - 3, pDst);
 			pDst += 2;
		}
	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pData);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	return pTexture;
}

//----------------------------------------------------------------------------
void Sample1::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mCuller.SetFrustum(mspCamera->GetFrustum());

	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	mpRenderer->ClearBuffers();
	mpRenderer->PreDraw(mspCamera);

	const UInt cubeCount = 5;
	const Float stride = 3.5F;
	Float offset = cubeCount * -0.5F * stride + stride * 0.5F;
	Float angle = mAngle;

	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.25F, 1, 0.5F), angle);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset, 0, 0));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			mpRenderer->Draw(mspCube);
		}

		angle += 0.2F;
		offset += stride;
	}

	mpRenderer->PostDraw();
	mpRenderer->DisplayBackBuffer();
}
