#include "Sample1.h"

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
	Vector3F cameraLocation(0.0F, 0.0F, 10.0F);
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
	const Float extent = 1.0F;
	const Vector3F vertices[] = {
		Vector3F(-extent, -extent, -extent),	// 0
		Vector3F(-extent,  extent, -extent),	// 1
		Vector3F( extent,  extent, -extent),	// 2
		Vector3F( extent, -extent, -extent),	// 3
		Vector3F( extent, -extent, extent),		// 4 (-3)
		Vector3F( extent,  extent, extent),		// 5 (-2)
		Vector3F(-extent,  extent, extent),		// 6 (-1)
		Vector3F(-extent, -extent, extent),		// 7 (-0)
		Vector3F( extent,  extent, extent),		// 8 (5)
		Vector3F(-extent,  extent, extent),		// 9 (6)
		Vector3F( extent,  extent, extent),		// 10(5)
		Vector3F( extent, -extent, extent),		// 11(4)
		Vector3F(-extent, -extent, extent),		// 12(7)
		Vector3F( extent, -extent, extent),		// 13(4)
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

		ColorRGB(1.0F, 0.0F, 0.0F),
		ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F),
		ColorRGB(0.0F, 1.0F, 1.0F),
	};

	const Float extentUv = 1.0F;
	const Vector2F uvs[] = {
		Vector2F(0.50F * extentUv, 0.50F * extentUv),
		Vector2F(0.50F * extentUv, 0.25F * extentUv),
		Vector2F(0.25F * extentUv, 0.25F * extentUv),
		Vector2F(0.25F * extentUv, 0.50F * extentUv),
		Vector2F(0.00F * extentUv, 0.50F * extentUv),
		Vector2F(0.00F * extentUv, 0.25F * extentUv),
		Vector2F(0.75F * extentUv, 0.25F * extentUv),
		Vector2F(0.75F * extentUv, 0.50F * extentUv),
		Vector2F(0.25F * extentUv, 0.00F * extentUv),
		Vector2F(0.50F * extentUv, 0.00F * extentUv),
		Vector2F(1.00F * extentUv, 0.25F * extentUv),
		Vector2F(1.00F * extentUv, 0.50F * extentUv),
		Vector2F(0.50F * extentUv, 0.75F * extentUv),
		Vector2F(0.25F * extentUv, 0.75F * extentUv)
	};

	const UInt indices[] = {
		0, 1, 2, 3,
		11, 10, 6, 7,
		7, 6, 1, 0,
		3, 2, 5, 4,
		1, 9, 8, 2,
		12, 0, 3, 13
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z

	UInt textureCount = 1;
	for (UInt unit = 0; unit < textureCount; unit++)
	{
		attributes.SetTCoordChannels(2, unit);	// channels: U, V
	}

	attributes.SetColorChannels(3);		// channels: R, G, B

	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(colors) / sizeof(ColorRGB)));	
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];

		for (UInt unit = 0; unit < textureCount; unit++)
		{
			pCubeVerts->TCoord2(i, unit) = uvs[i];
		}

		pCubeVerts->Color3(i) = colors[i];
	}

	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(6*6);
	for	(int i = 0; i < 6; i++)
	{
		(*pIndices)[0+i*6] = indices[0+i*4];
		(*pIndices)[1+i*6] = indices[1+i*4];
		(*pIndices)[2+i*6] = indices[3+i*4];

		(*pIndices)[3+i*6] = indices[3+i*4];
		(*pIndices)[4+i*6] = indices[1+i*4];
		(*pIndices)[5+i*6] = indices[2+i*4];
	}

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	Texture2D* pTexture = CreateTexture();
	pTextureEffect->Textures.Append(pTexture);
	pTextureEffect->BlendOps.Append(TextureEffect::BM_REPLACE);
	pCube->AttachEffect(pTextureEffect);

	return pCube;
}

#include "Sample1.inl"

//----------------------------------------------------------------------------
Texture2D* Sample1::CreateTexture()
{
	const UInt width = WireLogo64.width;
	const UInt height = WireLogo64.height;
	const UInt bpp = WireLogo64.bytes_per_pixel;

	UChar* pData = WIRE_NEW UChar[width * height * bpp];
	for (UInt i = 0; i < (width * height * bpp); i++)
	{
		pData[i] = WireLogo64.pixel_data[i];
	}

	Image2D* pImage = WIRE_NEW Image2D(Image2D::FM_RGB888, width, height, pData);
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
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), angle);
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
