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
	Vector3f cam(0.0f, 0.0f, 0.0f);
	Vector3f up(0.0f, 1.0f, 0.0f);
	Vector3f look(0.0f, 0.0f, -1.0f);

	mspCamera = WIRE_NEW Camera;
	Float width = static_cast<Float>(mpRenderer->GetWidth());
	Float height = static_cast<Float>(mpRenderer->GetHeight());
	mspCamera->LookAt(cam, up, look);
	mspCamera->SetFrustum(45, width / height , 0.1f, 300.0f);

	mRtri = 0.0f;
	mRquad = 0.0f;
	mAngle = 0.0f;

	return true;
}

//----------------------------------------------------------------------------
Geometry* Sample1::CreateCube()
{
	float extent = 1.0f;
	Vector3f vertices[] = {
		Vector3f(-extent, -extent, -extent),
		Vector3f(+extent, -extent, -extent),
		Vector3f(+extent, +extent, -extent),
		Vector3f(-extent, +extent, -extent),
		Vector3f(-extent, -extent, +extent),
		Vector3f(+extent, -extent, +extent),
		Vector3f(+extent, +extent, +extent),
		Vector3f(-extent, +extent, +extent)
	};

	ColorRGB colors[] = {
		ColorRGB(1.0f, 0.0f, 0.0f),
		ColorRGB(0.0f, 1.0f, 0.0f),
		ColorRGB(0.0f, 0.0f, 1.0f),
		ColorRGB(1.0f, 1.0f, 0.0f),
		ColorRGB(1.0f, 0.0f, 1.0f),
		ColorRGB(0.0f, 1.0f, 1.0f),
		ColorRGB(1.0f, 1.0f, 1.0f),
		ColorRGB(0.0f, 0.0f, 0.0f),
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(3);
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3f);
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
		ColorRGB(1.0f,0.0f,0.0f),			// Set The Color To Red
		ColorRGB(0.0f,1.0f,0.0f),			// Set The Color To Green
		ColorRGB(0.0f,0.0f,1.0f),			// Set The Color To Blue

		ColorRGB(1.0f,0.0f,0.0f),			// Set The Color To Red
		ColorRGB(0.0f,0.0f,1.0f),			// Set The Color To Blue
		ColorRGB(0.0f,1.0f,0.0f),			// Set The Color To Green

		ColorRGB(1.0f,0.0f,0.0f),			// Set The Color To Red
		ColorRGB(0.0f,0.0f,1.0f),			// Set The Color To Blue
		ColorRGB(0.0f,1.0f,0.0f),			// Set The Color To Green

		ColorRGB(1.0f,0.0f,0.0f),			// Set The Color To Red
		ColorRGB(0.0f,0.0f,1.0f),			// Set The Color To Blue
		ColorRGB(0.0f,1.0f,0.0f)			// Set The Color To Green
	};

	Vector3f vertices[] = {
		Vector3f( 0.0f, 1.0f, 0.0f),	// Top of Triangle (front)
		Vector3f(-1.0f,-1.0f, 1.0f),	// Left of Triangle (front)
		Vector3f( 1.0f,-1.0f, 1.0f),	// Right of Triangle (front)

		Vector3f( 0.0f, 1.0f, 0.0f),		// Top of Triangle (Right)
		Vector3f( 1.0f,-1.0f, 1.0f),	// Left of Triangle (Right)
		Vector3f( 1.0f,-1.0f,-1.0f),	// Right of Triangle (Right)

		Vector3f( 0.0f, 1.0f, 0.0f),		// Top of Triangle (Back)
		Vector3f(-1.0f,-1.0f,-1.0f),	// Left of Triangle (Back)
		Vector3f( 1.0f,-1.0f,-1.0f),	// Right of Triangle (Back)

		Vector3f( 0.0f, 1.0f, 0.0f),	// Top of Triangle (Left)
		Vector3f(-1.0f,-1.0f,-1.0f),	// Left of Triangle (Left)
		Vector3f(-1.0f,-1.0f, 1.0f)	// Right of Triangle (Left)
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
	Float scaleFactor = Mathf::Sin(mAngle);
	mAngle += Mathf::PI / 180.0f;
	mAngle = Mathf::FMod(mAngle, Mathf::PI);

	mpRenderer->BeginScene(mspCamera);

	Matrix34f model(Vector3f(0, -1, 0), Mathf::DEG_TO_RAD * mRtri);
	mspPyramid->Local.SetRotate(model);
	mspPyramid->Local.SetTranslate(Vector3f(-1.5f,0.0f,-6.0f));
	mspPyramid->Local.SetUniformScale(scaleFactor + 0.5f);
	mpRenderer->Draw(mspPyramid);

	model.FromAxisAngle(Vector3f(1, 1, 1), Mathf::DEG_TO_RAD * mRquad);
	mspCube->Local.SetRotate(model);
	mspCube->Local.SetTranslate(Vector3f(1.5f,0.0f,-7.0f));
	mspCube->Local.SetScale(Vector3f(scaleFactor + 0.5f, 1.0f, 1.0f));
	mpRenderer->Draw(mspCube);

	mpRenderer->EndScene();
	mpRenderer->DisplayBackBuffer();

	mRtri+=0.5f;		// Increase The Rotation Variable For The Triangle
	mRquad-=0.15f;	// Decrease The Rotation Variable For The Quad
}
