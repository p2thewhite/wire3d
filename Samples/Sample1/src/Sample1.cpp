#include "Sample1.h"
#include "WireMain.h"

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

	mRtri = 0.0F;
	mRquad = 0.0F;
	mAngle = 0.0F;

	return true;
}

//----------------------------------------------------------------------------
Geometry* Sample1::CreateCube()
{
	Float extent = 1.0F;
	Vector3F vertices[] = {
		Vector3F(-extent, -extent, -extent),
		Vector3F(+extent, -extent, -extent),
		Vector3F(+extent, +extent, -extent),
		Vector3F(-extent, +extent, -extent),
		Vector3F(-extent, -extent, +extent),
		Vector3F(+extent, -extent, +extent),
		Vector3F(+extent, +extent, +extent),
		Vector3F(-extent, +extent, +extent)
	};

	ColorRGB colors[] = {
		ColorRGB(1.0F, 0.0F, 0.0F),
		ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F),
		ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F),
		ColorRGB(0.0F, 0.0F, 0.0F),
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(3);
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		pCubeVerts->Color3(i) = colors[i];
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
	return pCube;
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
	mCuller.SetFrustum(mspCamera->GetFrustum());

	Float scaleFactor = MathF::Sin(mAngle);
	mAngle += MathF::PI / 180.0F;
	mAngle = MathF::FMod(mAngle, MathF::PI);

	mpRenderer->BeginScene(mspCamera);

	Float xOffset = -1.5F - mAngle * 4.0F;
	Matrix34F model(Vector3F(0, -1, 0), MathF::DEG_TO_RAD * mRtri);
	mspPyramid->World.SetRotate(model);
	mspPyramid->World.SetTranslate(Vector3F(xOffset, 0.0F, -6.0F));
	mspPyramid->World.SetUniformScale(scaleFactor + 0.5F);
	mspPyramid->ModelBound->TransformBy(mspPyramid->World, mspPyramid->WorldBound);
//	mspPyramid->UpdateWorldBound();	// is the same but protected
	if (!mCuller.IsVisible(mspPyramid->WorldBound))
	{
		mspCube->VBuffer->Color3(0) = ColorRGB::WHITE;
		mspCube->VBuffer->Color3(1) = ColorRGB::WHITE;
		mspCube->VBuffer->Color3(2) = ColorRGB::WHITE;
	}
	else
	{
		mspCube->VBuffer->Color3(0) = ColorRGB::RED;
		mspCube->VBuffer->Color3(1) = ColorRGB::GREEN;
		mspCube->VBuffer->Color3(2) = ColorRGB::BLUE;
		mpRenderer->Draw(mspPyramid);
	}

	model.FromAxisAngle(Vector3F(1, 1, 1), MathF::DEG_TO_RAD * mRquad);
	mspCube->World.SetRotate(model);
	mspCube->World.SetTranslate(Vector3F(1.5F, 0.0F, -7.0F));
	mspCube->World.SetScale(Vector3F(scaleFactor + 0.5f, 1.0F, 1.0F));
	mpRenderer->Draw(mspCube);

	mpRenderer->EndScene();
	mpRenderer->DisplayBackBuffer();

	mRtri += 0.5F;		// Increase The Rotation Variable For The Triangle
	mRquad -= 0.15F;	// Decrease The Rotation Variable For The Quad
}
