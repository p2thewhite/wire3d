#include "Sample2.h"

#include "CubeController.h"
#include "WireMain.h"

using namespace Wire;

WIRE_APPLICATION(Sample2);

//----------------------------------------------------------------------------
Sample2::Sample2()
	:
	WIREAPPLICATION(
		ColorRGBA(0.0F, 0.0F, 0.2F, 1.0F),	// Background color.
		"Sample2",							// Name of the application.
		0, 0,								// Window position and
		640, 480)							// size (both ignored on the Wii). 
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

	Geometry* pCube = CreateCube();
	CubeController* pCubeController = WIRE_NEW CubeController;
	pCube->AttachController(pCubeController);

	mspRoot = WIRE_NEW Node;
	mspRoot->AttachChild(pCube);
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

	mAngle = 0.0F;
	mLastTime = System::GetTime();

	return true;
}

//----------------------------------------------------------------------------
Geometry* Sample2::CreateCube()
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
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mCuller.SetFrustum(mspCamera->GetFrustum());

	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Geometry* pPyramid = static_cast<Geometry*>(mspRoot->GetChild(1).Get());

	Matrix34F model(Vector3F(0, -1, 0), mAngle);
	mspRoot->Local.SetRotate(model);
	mspRoot->Local.SetTranslate(Vector3F(0.0F, 0.0F, -8.0F));

	pPyramid->Local.SetRotate(model);
	pPyramid->Local.SetTranslate(Vector3F(-2.5F, 0.0F, 0.0F));

 	mspRoot->UpdateGS(time);
 	mCuller.ComputeVisibleSet(mspRoot);

	mpRenderer->ClearBuffers();
	mpRenderer->BeginScene(mspCamera);
	mpRenderer->DrawScene(mCuller.GetVisibleSet());
	mpRenderer->EndScene();
	mpRenderer->DisplayBackBuffer();
}
