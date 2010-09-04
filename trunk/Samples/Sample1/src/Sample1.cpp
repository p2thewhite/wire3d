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

	mspCullState = WIRE_NEW CullState;
	mspAlphaState = WIRE_NEW AlphaState;

	mAngle = 0.0F;
	mLastTime = System::GetTime();

	return true;
}

//----------------------------------------------------------------------------
Geometry* Sample1::CreateCube()
{
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

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z

	UInt textureCount = 1;
	for (UInt unit = 0; unit < textureCount; unit++)
	{
		attributes.SetTCoordChannels(2, unit);	// channels: U, V
	}

	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
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
	}

	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
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
	const UInt width = WireLogo.width;
	const UInt height = WireLogo.height;
	const UInt bpp = WireLogo.bytes_per_pixel;

	UChar* const pDst = WIRE_NEW UChar[width * height * 4];
	const UChar* pSrc = WireLogo.pixel_data;
	for (UInt y = 0; y < height; y++)
	{
		for (UInt x = 0; x < width; x++)
		{
			pDst[(y*width + x)*4] = *pSrc++;
			pDst[(y*width + x)*4+1] = *pSrc++;
			pDst[(y*width + x)*4+2] = *pSrc++;
			Float distance = 255.0F - Vector2F(x-width*0.5F, y-height*0.5F).
				Length() * 4 + 150;
			distance = distance < 0 ? 0 : distance;
			UChar alpha = distance > 255 ? 255 : static_cast<UChar>(distance);
			pDst[(y*width + x)*4+3] = alpha;
		}
	}

	Image2D* pImage = WIRE_NEW Image2D(Image2D::FM_RGBA8888, width, height,
		pDst);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetAnisotropyValue(4.0F);

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
	const Float offset = cubeCount * -0.5F * stride + stride * 0.6F;
	Float z = MathF::Sin(mAngle) * 3.0F;

	mspCullState->CullFace = CullState::CM_BACK;
	GetRenderer()->SetCullState(mspCullState);
	mspAlphaState->BlendEnabled = false;
	GetRenderer()->SetAlphaState(mspAlphaState);

	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), -mAngle - 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, 1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			mpRenderer->Draw(mspCube);
		}
	}


	mspCullState->CullFace = CullState::CM_FRONT;
	GetRenderer()->SetCullState(mspCullState);
	z = MathF::Cos(mAngle) * 3.0F;

	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			mpRenderer->Draw(mspCube);
		}
	}


	mspCullState->CullFace = CullState::CM_BACK;
	GetRenderer()->SetCullState(mspCullState);
	mspAlphaState->BlendEnabled = true;
	GetRenderer()->SetAlphaState(mspAlphaState);

	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			mpRenderer->Draw(mspCube);
		}
	}

	mpRenderer->PostDraw();
	mpRenderer->DisplayBackBuffer();
}
