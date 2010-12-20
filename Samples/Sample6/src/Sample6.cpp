// Sample6 - Multitexturing
// This sample demonstrates how to blend individual textures together in
// a single rendering pass using multi-texturing.

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

	// create a cube with vertex colors using a texture effect with 2 textures
	TextureEffect* pEffect = WIRE_NEW TextureEffect;
	pEffect->Textures.Append(CreateTexture());
	pEffect->Textures.Append(CreateTexture2());

	mspGeometry = CreateCube(pEffect->Textures.GetQuantity());
	mspGeometry->AttachEffect(pEffect);

	// camera and viewing frustum setup
	Vector3F cameraLocation(0.0F, 0.0F, -7.0F);
	Vector3F viewDirection(0.0F, 0.0F, 1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);

	// initializing working variables
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

	// rotate the cube based on the time passed	
	mAngle += static_cast<Float>(elapsedTime) * 0.5F;
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);
	Vector3F axis(Vector3F(MathF::Sin(mAngle), MathF::Cos(mAngle), 0.3F));
	Matrix34F model(axis, mAngle);
	mspGeometry->World.SetRotate(model);

	// define some blending modes for the cube to cycle through
	static const BlendMode modes[] =
	{
		// texture0 passes the previous stage (i.e. vertex color),
		// so does texture1, thus only the vertex color is visible
		BlendMode(TextureEffect::BM_PASS, TextureEffect::BM_PASS),

		// texture0 replaces the vertex color, texture1 passes the
		// the previous stage, thus only texture0 is visible
		BlendMode(TextureEffect::BM_REPLACE, TextureEffect::BM_PASS),

		// texture0 blends with the vertex color
		BlendMode(TextureEffect::BM_BLEND, TextureEffect::BM_PASS),

		// texture0 replaces the vertex color, texture1 is applied as a decal
		// over the previous stage (i.e. texture0)
		BlendMode(TextureEffect::BM_REPLACE, TextureEffect::BM_DECAL),

		// and so on... check WireTextureEffect.h for the formulas
		BlendMode(TextureEffect::BM_REPLACE, TextureEffect::BM_BLEND),
		BlendMode(TextureEffect::BM_MODULATE, TextureEffect::BM_BLEND),
		BlendMode(TextureEffect::BM_BLEND, TextureEffect::BM_BLEND)
	};
	
	// we change the blending mode every second
	UInt modeQuantity = sizeof(modes) / sizeof(BlendMode);
	UInt modeIndex = static_cast<UInt>(MathF::FMod(static_cast<Float>(time),
		static_cast<Float>(modeQuantity)));
	WIRE_ASSERT(0 <= modeIndex && modeIndex < modeQuantity);
	TextureEffect* pEffect = DynamicCast<TextureEffect>(mspGeometry->
		GetEffect());
	if (pEffect)
	{
		pEffect->BlendOps.SetElement(0, modes[modeIndex].Mode0);
		pEffect->BlendOps.SetElement(1, modes[modeIndex].Mode1);
	}

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->Draw(mspGeometry);
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample6::CreateCube(const UInt uvQuantity)
{
	// Creation of Wire::Geometry objects is explained in detail in Sample1

	const Vector3F vertices[] = {
		Vector3F(-1.0F, -1.0F, -1.0F), Vector3F(-1.0F,  1.0F, -1.0F),
		Vector3F( 1.0F,  1.0F, -1.0F), Vector3F( 1.0F, -1.0F, -1.0F),
		Vector3F( 1.0F, -1.0F, 1.0F), Vector3F( 1.0F,  1.0F, 1.0F),
		Vector3F(-1.0F,  1.0F, 1.0F), Vector3F(-1.0F, -1.0F, 1.0F),
		Vector3F( 1.0F,  1.0F, 1.0F), Vector3F(-1.0F,  1.0F, 1.0F),
		Vector3F( 1.0F,  1.0F, 1.0F), Vector3F( 1.0F, -1.0F, 1.0F),
		Vector3F(-1.0F, -1.0F, 1.0F), Vector3F( 1.0F, -1.0F, 1.0F)
	};

	const ColorRGB colors[] = {
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F), ColorRGB(0.0F, 0.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F)
	};

	const Vector2F uvs[] = {
		Vector2F(0.50F, 0.50F), Vector2F(0.50F, 0.25F),
		Vector2F(0.25F, 0.25F), Vector2F(0.25F, 0.50F),
		Vector2F(0.00F, 0.50F), Vector2F(0.00F, 0.25F),
		Vector2F(0.75F, 0.25F), Vector2F(0.75F, 0.50F),
		Vector2F(0.25F, 0.00F), Vector2F(0.50F, 0.00F),
		Vector2F(1.00F, 0.25F), Vector2F(1.00F, 0.50F),
		Vector2F(0.50F, 0.75F), Vector2F(0.25F, 0.75F)
	};

	const UInt indices[] = {
		0, 1, 2, 3,	11, 10, 6, 7, 7, 6, 1, 0, 3, 2, 5, 4, 1, 9, 8, 2, 12, 0,
		3, 13
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z

	for (UInt unit = 0; unit < uvQuantity; unit++)
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

		for (UInt unit = 0; unit < uvQuantity; unit++)
		{
			pCubeVerts->TCoord2(i, unit) = uvs[i];
		}

		pCubeVerts->Color3(i) = colors[i];
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
	return pCube;
}

//----------------------------------------------------------------------------
Texture2D* Sample6::CreateTexture()
{
	const UInt width = 512;
	const UInt height = 128;

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
	// create a 32bit checker board texture with alpha channel
	const UInt width = 64;
	const UInt height = 64;

	Image2D::FormatMode format = Image2D::FM_RGBA8888;
	const UInt bytesPerPixel = Image2D::GetBytesPerPixel(format);

	UChar* pData = WIRE_NEW UChar[width * height * bytesPerPixel];
	UChar* pDst = pData;

	for (UInt y = 0; y < height; y++)
	{
		Bool flip = true;
		if ((y & 0x4) > 0)
		{
			flip = false;
		}

		for (UInt x = 0; x < width; x++)
		{
			UChar t = flip ? 0x40 : 0x80;
			if ((x & 0x4) > 0)
			{
				t = flip ? 0x80 : 0x40;
			}

			*pDst++ = t;
			*pDst++ = t;
			*pDst++ = t;
			*pDst++ = 0x40;
		}
	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pData);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetFilterType(Texture2D::FT_NEAREST);
	return pTexture;
}
