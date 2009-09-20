#include "Sample2.h"
#include "WireMain.h"

using namespace Wire;

WIRE_APPLICATION(Sample2);

//----------------------------------------------------------------------------
Sample2::Sample2()
{
}

//----------------------------------------------------------------------------
Sample2::~Sample2()
{
}

//----------------------------------------------------------------------------
Bool Sample2::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspRoot = WIRE_NEW Node;
	mspRoot->AttachChild(CreateCube());
	mspRoot->AttachChild(CreatePyramid());

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
Geometry* Sample2::CreateCube()
{
	float extent = 1.0F;
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
Geometry* Sample2::CreatePyramid()
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
void Sample2::OnIdle()
{
	mCuller.SetFrustum(mspCamera->GetFrustum());

	Float scaleFactor = MathF::Sin(mAngle);
	mAngle += MathF::PI / 180.0F;
	mAngle = MathF::FMod(mAngle, MathF::PI);

	Geometry* pCube = static_cast<Geometry*>(mspRoot->GetChild(0).Get());
	Geometry* pPyramid = static_cast<Geometry*>(mspRoot->GetChild(1).Get());

	Matrix34F model(Vector3F(0, -1, 0), MathF::DEG_TO_RAD * mRtri);

	pPyramid->Local.SetRotate(model);
	pPyramid->Local.SetTranslate(Vector3F(-1.5F, 0.0F, -10.0F));
	pPyramid->Local.SetUniformScale(scaleFactor + 0.5F);

	model.FromAxisAngle(Vector3F(1, 1, 1), MathF::DEG_TO_RAD * mRquad);
	pCube->Local.SetRotate(model);
	pCube->Local.SetTranslate(Vector3F(1.5F, 0.0F, -7.0F));
	pCube->Local.SetScale(Vector3F(scaleFactor + 0.5f, 1.0F, 1.0F));

	mspRoot->Local.SetTranslate(Vector3F::UNIT_X * scaleFactor * 3.0F);
		
 	mspRoot->UpdateGS();
 	mCuller.ComputeVisibleSet(mspRoot);

	mpRenderer->BeginScene(mspCamera);
	mpRenderer->DrawScene(mCuller.GetVisibleSet());
	mpRenderer->EndScene();
	mpRenderer->DisplayBackBuffer();

	mRtri += 0.5F;		// Increase The Rotation Variable For The Triangle
	mRquad -= 0.15F;	// Decrease The Rotation Variable For The Quad
}
