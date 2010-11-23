#include "Sample6.h"

using namespace Wire;

WIRE_APPLICATION(Sample6);

//----------------------------------------------------------------------------
Bool Sample6::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	TArray<Texture2D*> textures;
	mspRoot = WIRE_NEW Node;
	mspRoot->AttachChild(CreateCube(textures));
	textures.Append(CreateTexture());
	mspRoot->AttachChild(CreateCube(textures));
 	textures.Append(CreateTexture2());
	mspRoot->AttachChild(CreateCube(textures, false));

	UInt cubeCount = mspRoot->GetQuantity();
	Float stride = 3.0F;
	Float offset = static_cast<Float>(cubeCount) * -0.5F * stride + stride * 0.5F;

	for (UInt i = 0; i < cubeCount; i++)
	{
		Spatial* pCube = mspRoot->GetChild(i);
		pCube->Local.SetTranslate(Vector3F(offset, 0, 0));
		offset += stride;
	}

	Vector3F cameraLocation(0.0F, 0.0F, -10.0F);
	Vector3F viewDirection(0.0F, 0.0F, 1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);

	mCuller.SetCamera(mspCamera);

	mAngle = 0.0F;

	mLastTime = System::GetTime();

	return true;
}

//----------------------------------------------------------------------------
void Sample6::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	// 	Float scaleFactor = MathF::Sin(mAngle * 2.0F) * 0.8F;
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Matrix34F model(Vector3F(0, 1, 0), mAngle);
	mspRoot->Local.SetRotate(model);
	mspRoot->Local.SetTranslate(Vector3F::ZERO);

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSet());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample6::CreateCube(TArray<Texture2D*>& rTextures,
	Bool useVertexColors)
{
	// Creation of Wire::Geometry objects is explained in detail in Sample1

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

	UInt textureCount = rTextures.GetQuantity();
	for (UInt unit = 0; unit < textureCount; unit++)
	{
		attributes.SetTCoordChannels(2, unit);	// channels: U, V
	}

	if (useVertexColors)
	{
		attributes.SetColorChannels(3);		// channels: R, G, B
	}

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

		if (useVertexColors)
		{
			pCubeVerts->Color3(i) = colors[i];
		}
	}

	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(6*6);
	for	(UInt i = 0; i < 6; i++)
	{
		(*pIndices)[0+i*6] = indices[0+i*4];
		(*pIndices)[1+i*6] = indices[1+i*4];
		(*pIndices)[2+i*6] = indices[3+i*4];

		(*pIndices)[3+i*6] = indices[3+i*4];
		(*pIndices)[4+i*6] = indices[1+i*4];
		(*pIndices)[5+i*6] = indices[2+i*4];
	}

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);

	if (textureCount > 0)
	{
		TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
		pCube->AttachEffect(pTextureEffect);

		for (UInt i = 0; i < textureCount; i++)
		{
			pTextureEffect->Textures.Append(rTextures[i]);
			TextureEffect::BlendMode blendMode = TextureEffect::BM_MODULATE;
			if (!useVertexColors && (i == 0))
			{
				blendMode = TextureEffect::BM_REPLACE;
			}

			pTextureEffect->BlendOps.Append(blendMode);
		}
	}

	return pCube;
}

//----------------------------------------------------------------------------
Texture2D* Sample6::CreateTexture()
{
	const UInt width = 256;
	const UInt height = 64;

	UChar* pMap = WIRE_NEW UChar[width * height];
	UInt seed = 7;
	for (UInt x = 0; x < width; x++)
	{
		pMap[x] = seed;
	}

	for (UInt i = width; i < (width * height)-1; i++)
	{
		seed *= 0x06255;
		seed = (seed >> 7) | (seed << (32-7));
		UInt val = (7 & seed) - 1;
		UChar texel = (pMap[i-width] + pMap[i-width+1]) >> 1;
		pMap[i] = val + texel;
	}

	Image2D::FormatMode format = Image2D::FM_RGB565;
	const UInt bytesPerPixel = Image2D::GetBytesPerPixel(format);

	UChar* pData = WIRE_NEW UChar[width * height * bytesPerPixel];
	UChar* pDst = pData;

	for (UInt y = 0; y < height; y++)
	{
		UInt temp;
		for (UInt x = 0; x < width; x++)
		{
			UChar t = pMap[y*width+x];
			UChar* pTemp = reinterpret_cast<UChar*>(&temp);
			*pTemp++ = t;
			*pTemp++ = t;
			*pTemp++ = t;
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
Texture2D* Sample6::CreateTexture2()
{
	const UInt width = 64;
	const UInt height = 64;

	Image2D::FormatMode format = Image2D::FM_RGB888;
	const UInt bytesPerPixel = Image2D::GetBytesPerPixel(format);

	UChar* pData = WIRE_NEW UChar[width * height * bytesPerPixel];
	UChar* pDst = pData;

	for (UInt y = 0; y < height; y++)
	{
		bool flip = true;
		if ((y & 0x4) > 0)
		{
			flip = false;
		}

		for (UInt x = 0; x < width; x++)
		{
			UChar t = flip ? 0xc0 : 0xFF;
			if ((x & 0x4) > 0)
			{
				t = flip ? 0xFF : 0xc0;
			}

			*pDst++ = t;
			*pDst++ = t;
			*pDst++ = t;
		}
	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pData);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetFilterType(Texture2D::FT_NEAREST);
	return pTexture;
}
