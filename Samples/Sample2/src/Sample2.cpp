#include "Sample2.h"

#include "CubeController.h"

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

	mspRoot = WIRE_NEW Node;

	Geometry* pBody1 = CreateCube();
	pBody1->Local.SetScale(Vector3F(2, 1, 1));
	mspRoot->AttachChild(pBody1);

	Geometry* pBody2 = CreateCube();
	pBody2->Local.SetScale(Vector3F(1, 2, 1));
	pBody2->Local.SetTranslate(Vector3F(3, 1, 0));
	pBody2->VBuffer->Position3(1).Y() = -0.5F;
	pBody2->VBuffer->Position3(5).Y() = -0.5F;
	mspRoot->AttachChild(pBody2);

	Geometry* pTail = CreateCube();
	pTail->Local.SetTranslate(Vector3F(5, 2, 0));
	pTail->VBuffer->Position3(2) = Vector3F(5, 1, -0.25F);
	pTail->VBuffer->Position3(6) = Vector3F(5, 1, 0.25F);
	pTail->VBuffer->Position3(1) = Vector3F(5, 0.5F, -0.25F);
	pTail->VBuffer->Position3(5) = Vector3F(5, 0.5F, 0.25F);
	mspRoot->AttachChild(pTail);

	Geometry* pNose = CreateCube();
	pNose->Local.SetTranslate(Vector3F(-3, 0, 0));
	pNose->VBuffer->Position3(0) = Vector3F(-1, -0.75F, -0.35F);
	pNose->VBuffer->Position3(4) = Vector3F(-1, -0.75F, 0.35F);
	pNose->VBuffer->Position3(3) = Vector3F(-1, -0.25F, -0.35F);
	pNose->VBuffer->Position3(7) = Vector3F(-1, -0.25F, 0.35F);
	mspRoot->AttachChild(pNose);

	Node* pRear = WIRE_NEW Node;
	mspRoot->AttachChild(pRear);
	pRear->Local.SetTranslate(Vector3F(10, 2.75F, 0.55F));

	mspRearRotor = CreateCube();
	mspRearRotor->Local.SetScale(Vector3F(1, 0.1F, 0.1F));
	pRear->AttachChild(mspRearRotor);

	mspRoot->Local.SetTranslate(Vector3F(0.0F, 0.0F, -20.0F));

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

	ColorRGBA colors[] = {
		ColorRGBA(1.0F, 0.0F, 0.0F, 1.0F),
		ColorRGBA(0.0F, 1.0F, 0.0F, 1.0F),
		ColorRGBA(0.0F, 0.0F, 1.0F, 0.0F),
		ColorRGBA(1.0F, 1.0F, 0.0F, 0.0F),
		ColorRGBA(1.0F, 0.0F, 1.0F, 1.0F),
		ColorRGBA(0.0F, 1.0F, 1.0F, 1.0F),
		ColorRGBA(1.0F, 1.0F, 1.0F, 0.0F),
		ColorRGBA(0.0F, 0.0F, 0.0F, 0.0F),
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(4);
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		pCubeVerts->Color4(i) = colors[i];
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

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
	return pCube;
}

//----------------------------------------------------------------------------
void Sample2::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	if (time > 2)
	{
//		WIRE_ASSERT(false);
	}
	
	System::Print("%f, %f\n", static_cast<Float>(time),
 		static_cast<Float>(elapsedTime));

	mCuller.SetFrustum(mspCamera->GetFrustum());

	mAngle += static_cast<Float>(elapsedTime * 2.0F);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Matrix34F model(Vector3F(0, -1, 0), mAngle);
	mspRoot->Local.SetRotate(model);
//	mspRoot->Local.SetTranslate(Vector3F(0, 0, -1) * mAngle + Vector3F(0, 0, -5));

	Matrix34F rearRotation(Vector3F(0, 0, 1), MathF::FMod(mAngle * 8,
		MathF::TWO_PI));
	mspRearRotor->Local.SetRotate(rearRotation);

 	mspRoot->UpdateGS(time);
 	mCuller.ComputeVisibleSet(mspRoot);

	mpRenderer->ClearBuffers();
	mpRenderer->PreDraw(mspCamera);
	mpRenderer->DrawScene(mCuller.GetVisibleSet());
	mpRenderer->PostDraw();
	mpRenderer->DisplayBackBuffer();
}
