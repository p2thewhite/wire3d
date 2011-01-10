#include "LensflareNode.h"
#include "WireFoundation.h"

using namespace Wire;

//----------------------------------------------------------------------------
LensflareNode::LensflareNode()
{
	CreateFlares();

	// Never let this node be culled by the system, because we determine
	// the visibility of this node and its children (i.e. flares) ourselves.
	Culling = CULL_NEVER;
}

//----------------------------------------------------------------------------
void LensflareNode::GetVisibleSet(Culler& rCuller, Bool noCull)
{
	const Camera* pCam = rCuller.GetCamera();
//	LightPtr spLight = 

	Transformation camTrafo;
	camTrafo.SetTranslate(pCam->GetLocation());
	camTrafo.SetRotate(Matrix3F(pCam->GetRVector(), pCam->GetUVector(),
		pCam->GetDVector(), true));

	const Vector3F& rCamPosition = camTrafo.GetTranslate();
	Vector3F lightDirection = Vector3F(0, 0, 1) * 500.0F;
 	Vector3F lightPosWorld = rCamPosition + lightDirection;
	lightPosWorld = Vector3F(20, 20, -500);

	Float l, r, b, t, n, f;
	pCam->GetFrustum(l, r, b, t, n, f);

	Vector4F col0((2*n)/(r-l),	0.0F,			0.0F,			0.0F);
	Vector4F col1(0.0F,			2*n/(t-b),		0.0F,			0.0F);
	Vector4F col2((r+l)/(r-l),	(t+b)/(t-b),	-(f+n)/(f-n),	-1.0F);
	Vector4F col3(0.0F,			0.0F,			-2*(f*n)/(f-n),	0.0F);
	Matrix4F projectionMatrix;
	projectionMatrix.SetColumn(0, col0);
	projectionMatrix.SetColumn(1, col1);
	projectionMatrix.SetColumn(2, col2);
	projectionMatrix.SetColumn(3, col3);

	// transform to camera space
	Vector3F lightPosCamSpace = camTrafo.ApplyInverse(lightPosWorld);

	// transform to screen space
	Vector4F lightPosScreenSpace = projectionMatrix * Vector4F(
		lightPosCamSpace.X(), lightPosCamSpace.Y(), lightPosCamSpace.Z(), 1);

	float invW = 1.0F / lightPosScreenSpace.W();

	//to screen coordinates [1,1 | -1,-1]
	Vector2F lightPos2D = Vector2F((lightPosScreenSpace.X() * invW * -1),
		(lightPosScreenSpace.Y() * invW * -1));

	//to frustum coordinates [fL - fR | fT - fB]
	lightPos2D[0] *= r;
	lightPos2D[1] *= t;

	Float position = 1.0F;
	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Vector3F newPos = pCam->GetLocation() +
			pCam->GetDVector() * (n + (0.01F * (GetQuantity()-i))) +
			pCam->GetRVector() * lightPos2D[0] * position +
			pCam->GetUVector() * lightPos2D[1] * position;

		position -= 0.5F;

		//set new position
		GetChild(i)->Local.SetTranslate(newPos);

		//set rotation from camera
		GetChild(i)->Local.SetRotate(camTrafo.GetRotate());
	}	

	Node::GetVisibleSet(rCuller, noCull);
}

//----------------------------------------------------------------------------
void LensflareNode::CreateFlares()
{
	CreateTextures();

	AttachChild(CreateQuad(0.01F, 0.498F, 0.0F, 0.0F, mspLensTex0));
	AttachChild(CreateQuad(0.01F, 0.5F, 0.501F, 0.501F, mspLensTex0));
	AttachChild(CreateQuad(0.01F, 0.5F, 0.501F, 0.0F, mspLensTex0));
	AttachChild(CreateQuad(0.01F, 0.5F, 0.0F, 0.5F, mspLensTex0));
	AttachChild(CreateQuad(0.01F, 1, 0, 0, mspLensTex1));

	StateAlpha* pAlpha = WIRE_NEW StateAlpha;
	pAlpha->BlendEnabled = true;
	AttachState(pAlpha);

	StateZBuffer* pZBuffer = WIRE_NEW StateZBuffer;
	pZBuffer->Writable = false;
	pZBuffer->Enabled = false;
	AttachState(pZBuffer);
	
	UpdateRS();
}

//----------------------------------------------------------------------------
Geometry* LensflareNode::CreateQuad(Float scale, Float uvFactor,
	Float uOffset, Float vOffset, Texture2D* pTexture)
{
	Geometry* pQuad = StandardMesh::CreateQuad(0, 1, false, scale);
	VertexBuffer* pVBuffer = pQuad->VBuffer;

	for (UInt i = 0; i < pVBuffer->GetVertexQuantity(); i++)
	{
		pVBuffer->TCoord2(i) *= uvFactor;
 		pVBuffer->TCoord2(i).X() += uOffset;
 		pVBuffer->TCoord2(i).Y() += vOffset;
	}

	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	pTextureEffect->Textures.Append(pTexture);
	pTextureEffect->BlendOps.Append(TextureEffect::BM_REPLACE);
	pQuad->AttachEffect(pTextureEffect);

	return pQuad;
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
	for (UInt count = 0; count < 300; count++)
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
		Float f = pTemp[i] * max * 5000.0F;
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
