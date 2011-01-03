#include "LensflareNode.h"
#include "WireFoundation.h"

using namespace Wire;

//----------------------------------------------------------------------------
LensflareNode::LensflareNode()
{
	CreateTextures();

	AttachChild(CreateQuad(0.498F, 0.0F, 0.0F, mspLensTex0));
	AttachChild(CreateQuad(0.5F, 0.501F, 0.501F, mspLensTex0));
	AttachChild(CreateQuad(0.5F, 0.501F, 0.0F, mspLensTex0));
	AttachChild(CreateQuad(0.5F, 0.0F, 0.5F, mspLensTex0));
	AttachChild(CreateQuad(1, 0, 0, mspLensTex1));

	const UInt count = GetQuantity();
	const Float stride = 3.5F;
	const Float offset = count * -0.5F * stride + stride * 0.6F;

	for (UInt i = 0; i < count; i++)
	{
		GetChild(i)->Local.SetTranslate(Vector3F(offset + stride * i, 0, -10));

	}
}

//----------------------------------------------------------------------------
Geometry* LensflareNode::CreateQuad(Float uvFactor, Float uOffset,
	Float vOffset, Texture2D* pTexture)
{
	const Vector3F vertices[] =
	{
		Vector3F(-1, 1, 0), Vector3F(1, 1, 0),
		Vector3F(1, -1, 0), Vector3F(-1, -1, 0)
	};

	// Texture coordinates
	const Vector2F uvs[] =
	{
		// side 1
		Vector2F(0.0F, 0.0F), Vector2F(1.0F, 0.0F),
		Vector2F(1.0F, 1.0F), Vector2F(0.0F, 1.0F)
	};

	// Indices provide connectivity information and define the triangle mesh.
	// Every side of the cube consists of 2 triangles.
	const UInt indices[] =
	{
		// side 1
		0, 2, 1,
		0, 3, 2,
	};

	// Before creating the VertexBuffer we need to define its format.
	// It consists of 3d positions and 2d texture coordinates
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetTCoordChannels(2);	// channels: U, V

	// Now with the attributes being defined, we can create a VertexBuffer
	// and fill it with data.
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		pCubeVerts->TCoord2(i) = uvs[i] * uvFactor;
 		pCubeVerts->TCoord2(i).X() += uOffset;
 		pCubeVerts->TCoord2(i).Y() += vOffset;
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

	// The cube shall be textured. Therefore we create and attach a texture
	// effect, where we add a texture and define its blending mode.
	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	pTextureEffect->Textures.Append(pTexture);
	pTextureEffect->BlendOps.Append(TextureEffect::BM_REPLACE);
	pCube->AttachEffect(pTextureEffect);

	return pCube;
}

//----------------------------------------------------------------------------
void LensflareNode::CreateTextures()
{
	const Image2D::FormatMode format = Image2D::FM_RGBA8888;
	const UInt bpp = Image2D::GetBytesPerPixel(format);
	UInt width = 1024;
	UInt height = 1024;
	Float r = MathF::Min(static_cast<Float>(width/2), static_cast<Float>(
		height/2)) * 0.5F;

	UChar* pDst = WIRE_NEW UChar[width * height * bpp];
	UChar* pDst0 = pDst;
	UChar* pDst1 = pDst + width/2*bpp;
	UChar* pDst2 = pDst + width*height/2*bpp;
	UChar* pDst3 = pDst + width*height/2*bpp + width/2*bpp;

	for (UInt y = 0; y < height/2; y++)
	{
		for (UInt x = 0; x < width/2; x++)
		{
			const Float dx = r-x;
			const Float dy = r-y;
			const Float r1 = MathF::Sqrt(dx*dx + dy*dy) / r;

			// replace the if-statement with c *= 1-smoothstep(1-.01, 1+.01, r);

			Float c = 1-r1;
			c = c*c;
			c *= 1 - SmoothStep(1-0.01F, 1+0.01F, r1);
			UChar c0 = static_cast<UChar>(c * 255.0F);
			*pDst0++ = c0;
			*pDst0++ = c0;
			*pDst0++ = c0;
			*pDst0++ = c0;

			c = r1*r1;
			c = c*c;
			c = c*c*c;
			c *= 1 - SmoothStep(1-0.01F, 1+0.01F, r1);
			UChar c1 = static_cast<UChar>(c * 255.0F);
			*pDst1++ = c1;
			*pDst1++ = c1;
			*pDst1++ = c1;
			*pDst1++ = c1;

			c = r1;
			c *= 1 - SmoothStep(1-0.01F, 1+0.01F, r1);
			UChar c2 = static_cast<UChar>(c * 255.0F);
			*pDst2++ = c2;
			*pDst2++ = c2;
			*pDst2++ = c2;
			*pDst2++ = c2;

			c = 1-MathF::FAbs(r1 - 0.9F) / 0.1F;
			if (c < 0)
			{
				c = 0;
			}
			else
			{
				c = c*c;
				c = c*c;
			}

			UChar c3 = static_cast<UChar>(c * 255.0F);
			*pDst3++ = c3;
			*pDst3++ = c3;
			*pDst3++ = c3;
			*pDst3++ = c3;
		}

		pDst0 += width/2*bpp;
		pDst1 += width/2*bpp;
		pDst2 += width/2*bpp;
		pDst3 += width/2*bpp;
	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst);
	mspLensTex0 = WIRE_NEW Texture2D(pImage);


	// 
 	width = 256;
 	height = 256;
	r = MathF::Min(static_cast<Float>(width), static_cast<Float>(height))
		* 0.5F;
	pDst = WIRE_NEW UChar[width * height * bpp];
	Float* const pTemp = WIRE_NEW Float[width*height];
	for (UInt i = 0; i < width*height; i++)
	{
		pTemp[i] = 0.0F;
	}

	Random rand;
	for (UInt count = 0; count < 200; count++)
	{
		// pick a random direction
		Float angle = rand.GetFloat() * MathF::TWO_PI;
		Float dx = MathF::Cos(angle);
		Float dy = MathF::Sin(angle);

		// push particle along this path
		Float fx = static_cast<Float>(width) * 0.5F;
		Float fy = static_cast<Float>(height) * 0.5F;

		for (UInt step = 0; step < r; step++)
		{
			DrawParticle(pTemp, fx, fy, width);
			fx += dx;
			fy += dy;
		}
	}

	// normalize
	Float max = 0;
	for (UInt i = 0; i < width*height; i++)
	{
		if (pTemp[i] > max)
		{
			max = pTemp[i];
		}
	}

	max = 1/max;
	for (UInt i = 0; i < width*height; i++)
	{
		Float f = pTemp[i] * max * 300.0F;
		UChar val = static_cast<UChar>(MathF::Clamp(0, f, 255.0F));
		pDst[i*bpp] = val;
		pDst[i*bpp+1] = val;
		pDst[i*bpp+2] = val;
		pDst[i*bpp+3] = val;
	}

	pImage = WIRE_NEW Image2D(format, width, height, pDst);
	mspLensTex1 = WIRE_NEW Texture2D(pImage);
}

//----------------------------------------------------------------------------
void LensflareNode::DrawParticle(Float* const pDst, Float fx, Float fy,
	UInt width)
{
	const Int partRadius = 3;

	for (Int y = -partRadius; y < partRadius; y++)
	{
		for (Int x = -partRadius; x < partRadius; x++)
		{
			Float r2 = static_cast<Float>(x*x + y*y);
			Float c = 1 - r2 / static_cast<Float>(partRadius*partRadius);
			c = c*c;
			c = c*c;
			pDst[(y+static_cast<Int>(fy))*width+ x+static_cast<Int>(fx)] += c;
		}
	}
}

//----------------------------------------------------------------------------
Float LensflareNode::SmoothStep(Float a, Float b, Float x)
{
	if (x < a)
	{
		return 0;
	}

	if (x >= b)
	{
		return 1;
	}

	x = (x-a) / (b-a);
	return (x*x) * (3-2*x);
}
