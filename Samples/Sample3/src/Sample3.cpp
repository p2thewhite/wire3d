#include "Sample3.h"

using namespace Wire;

WIRE_APPLICATION(Sample3);

//----------------------------------------------------------------------------
Bool Sample3::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspRoot = WIRE_NEW Node;
	Node* pLeft = CreateLods();
	mspRoot->AttachChild(pLeft);

	Node* pRight = CreateLods();
	mspRoot->AttachChild(pRight);

	WireframeState* pWireframe = WIRE_NEW WireframeState;
	pWireframe->Enabled = true;
	pRight->AttachGlobalState(pWireframe);

	CullState* pCull = WIRE_NEW CullState;
	pCull->CullFace = CullState::CM_OFF;
	pRight->AttachGlobalState(pCull);

	mspRoot->UpdateRS();
	Renderer::BindAll(mspRoot);

	// setup our camera at the origin
	// looking down the -z axis with y up
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
void Sample3::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Matrix34F rotate(Vector3F(1, 1, 0), mAngle);

	Spatial* pLeft = mspRoot->GetChild(0);
	pLeft->Local.SetRotate(rotate);
	pLeft->Local.SetTranslate(Vector3F(-2, 0, MathF::Sin(mAngle) * 7));

	Spatial* pRight = mspRoot->GetChild(1);
	pRight->Local.SetRotate(rotate);
	pRight->Local.SetTranslate(Vector3F(2, 0, MathF::Sin(mAngle) * 7));

 	mspRoot->UpdateGS(time);
 	mCuller.ComputeVisibleSet(mspRoot);

	mpRenderer->ClearBuffers();
	mpRenderer->PreDraw(mspCamera);
	mpRenderer->DrawScene(mCuller.GetVisibleSet());
	mpRenderer->PostDraw();
	mpRenderer->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
DLodNode* Sample3::CreateLods()
{
	DLodNode* pLod = WIRE_NEW DLodNode;

	pLod->AttachChild(CreateGeometry(14, 192));
 	pLod->AttachChild(CreateGeometry(10, 192));
 	pLod->AttachChild(CreateGeometry(8, 128));
	pLod->AttachChild(CreateGeometry(6, 96));
	
	pLod->SetModelDistance(0, 0, 5);
	pLod->SetModelDistance(1, 5, 10);
	pLod->SetModelDistance(2, 10, 15);
	pLod->SetModelDistance(3, 15, 100);

	pLod->ModelCenter() = Vector3F::ZERO;
	return pLod;
}

//----------------------------------------------------------------------------
Geometry* Sample3::CreateGeometry(UInt shapeCount, UInt segmentCount)
{
	Geometry* pTorus = CreatePqTorusKnot(shapeCount, 0.2F, segmentCount, 4, 3);
	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	pTextureEffect->Textures.Append(CreateTexture());
	pTextureEffect->BlendOps.Append(TextureEffect::BM_REPLACE);
	pTorus->AttachEffect(pTextureEffect);

	return pTorus;
}

//----------------------------------------------------------------------------
Geometry* Sample3::CreatePqTorusKnot(UInt shapeCount, Float shapeRadius,
	UInt segmentCount, UInt p, UInt q)
{
	shapeCount++;
	segmentCount++;

	TArray<Vector3F> shape(shapeCount);

	Vector3F pos(0, shapeRadius, 0);
	Float angleStride = MathF::TWO_PI / (shapeCount-1);
	Float angle = 0;
	for (UInt i = 0; i < (shapeCount-1); i++)
	{
		Matrix34F rot(Vector3F(0, 0, 1), angle);
		shape.SetElement(i, pos * rot);
		angle += angleStride;
	}

	shape.SetElement(shapeCount-1, shape[0]);

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetTCoordChannels(2);	// channels: U, V

	const UInt vertexCount = segmentCount * shapeCount;
	VertexBuffer* pVertices = WIRE_NEW VertexBuffer(attributes, vertexCount);

	angleStride = MathF::TWO_PI / (segmentCount-1);
	angle = 0;
	for (UInt i = 0; i < segmentCount-1; i++)
	{
		Float r = 0.5F * (2 + MathF::Sin(q * angle));
		Float x = r * MathF::Cos(p * angle);
		Float y = r * MathF::Cos(q * angle);
		Float z = r * MathF::Sin(p * angle);
		Vector3F p0(x, y, z);

		r = 0.5F * (2 + MathF::Sin(q * (angle+0.1F)));
		x = r * MathF::Cos(p * (angle+0.1F));
		y = r * MathF::Cos(q * (angle+0.1F));
		z = r * MathF::Sin(p * (angle+0.1F));
		Vector3F p1(x, y, z);

		angle += angleStride;		

		Vector3F t = p1-p0;
		Vector3F n = p1+p0;
		Vector3F b = t.Cross(n);
		n = b.Cross(t);
		n.Normalize();
		b.Normalize();

		for (UInt j = 0; j < shapeCount; j++)
		{	
			Vector3F newVertex = p0 + shape[j].X() * n + shape[j].Y() * b;
			pVertices->Position3(i*shapeCount + j) = newVertex;

			Vector2F uv((1.0F/(shapeCount-1))*j, (1.0F/(segmentCount-1))*i);
			pVertices->TCoord2(i*shapeCount + j) = uv;
		}
	}

	for (UInt j = 0; j < shapeCount; j++)
	{	
		pVertices->Position3((segmentCount-1)*shapeCount + j) = pVertices->
			Position3(j);
		Vector2F uv = pVertices->TCoord2(j);
		uv.Y() += 1.0F;
		pVertices->TCoord2((segmentCount-1)*shapeCount + j) = uv;
	}

	const UInt indexCount = (segmentCount-1)*(shapeCount-1)*6;
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexCount);
	for (UInt j = 0; j < segmentCount-1; j++)
	{
		UInt offset = shapeCount*j;
		for (UInt i = 0; i < shapeCount-1; i++)
		{
			UInt index = (shapeCount-1)*j+i;
			UInt index0 = i+offset;
			UInt index1 = index0+shapeCount;
			UInt index2 = index0+1;
			UInt index3 = index0+shapeCount+1;

			(*pIndices)[index*6] = index0;
			(*pIndices)[index*6+1] = index1;
			(*pIndices)[index*6+2] = index2;

			(*pIndices)[index*6+3] = index2;
			(*pIndices)[index*6+4] = index1;
			(*pIndices)[index*6+5] = index3;
		}
	}

	Geometry* pGeo = WIRE_NEW Geometry(pVertices, pIndices);
	return pGeo;
}

//----------------------------------------------------------------------------
struct Cell
{
	Vector2F point;
	ColorRGB color;
};

//----------------------------------------------------------------------------
Texture2D* Sample3::CreateTexture()
{
	if (mspTexture)
	{
		return mspTexture;
	}

	// Define the properties of the image to be used as a texture
	const UInt width = 64;
	const UInt height = 64;
	const Image2D::FormatMode format = Image2D::FM_RGB888;
	const UInt bpp = Image2D::GetBytesPerPixel(format);
	UChar* const pDst = WIRE_NEW UChar[width * height * bpp];

	TArray<Cell> cells;
	Random random;
	for (UInt i = 0; i < 30; i++)
	{
		Cell cell;
		cell.point.X() = random.GetFloat() * width;
		cell.point.Y() = random.GetFloat() * height;
		cell.color.R() = random.GetFloat() * 4;
		cell.color.G() = random.GetFloat() * 4;
		cell.color.B() = random.GetFloat() * 4;

		cells.Append(cell);
	}

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

			float factor = (min2Dist - minDist) + 20;
			ColorRGB color = cells[minIndex].color * factor;
			pDst[(y*width+x)*bpp] = static_cast<UChar>(color.R());
			pDst[(y*width+x)*bpp+1] = static_cast<UChar>(color.G());
			pDst[(y*width+x)*bpp+2] = static_cast<UChar>(color.B());
		}
	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetWrapType(0, Texture2D::WT_REPEAT);
	pTexture->SetWrapType(1, Texture2D::WT_REPEAT);

	mspTexture = pTexture;
	return pTexture;
}
