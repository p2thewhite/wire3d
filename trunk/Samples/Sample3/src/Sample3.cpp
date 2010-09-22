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
	mspRoot->AttachChild(Create());

	// setup our camera at the origin
	// looking down the -z axis with y up
	Vector3F cameraLocation(0.0F, 1.0F, 25.0F);
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
Geometry* Sample3::Create()
{
	const UInt vertexCount = 10;
	TArray<Vector3F> shape(vertexCount);

	Vector3F pos(0, 1, 0);
	Float angleStride = MathF::TWO_PI / vertexCount;
	Float angle = 0;
	for (UInt i = 0; i < vertexCount; i++)
	{
		Matrix34F rot(Vector3F(1, 0, 0), angle);
		shape.SetElement(i, pos * rot + Vector3F(0, 5, 0));
		angle += angleStride;
	}

	const UInt segmentCount = 10;
	const UInt totalCount = segmentCount * vertexCount;
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	//	attributes.SetTCoordChannels(2);	// channels: U, V

	VertexBuffer* pVertices = WIRE_NEW VertexBuffer(attributes, totalCount);

	angleStride = MathF::TWO_PI / segmentCount;
	angle = 0;
	for (UInt i = 0; i < segmentCount; i++)
	{
		Matrix34F rot(Vector3F(0, 0, 1), angle);
		angle += angleStride;

		for (UInt j = 0; j < vertexCount; j++)
		{	
			pVertices->Position3(i*vertexCount + j) = shape[j] * rot;
		}
	}


	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(totalCount*6);

	for (UInt j = 0; j < segmentCount; j++)
	{
		UInt offset = vertexCount*j;
		for (UInt i = 0; i < vertexCount; i++)
		{
			UInt index0 = i+offset;
			UInt index1 = i+vertexCount+offset;
			UInt index2 = i+1+offset;
			UInt index3 = i+vertexCount+1+offset;

			index1 = index1 >= totalCount ? index1-totalCount : index1;
			index3 = index3 >= totalCount ? index3-totalCount : index3;

			if (index2 >= vertexCount+offset)
			{
				index2 -= vertexCount;
			}

			if (index3 >= vertexCount*2+offset)
			{
				index3 -= vertexCount;
			}

			(*pIndices)[(offset+i)*6] = index0;
			(*pIndices)[(offset+i)*6+1] = index1;
			(*pIndices)[(offset+i)*6+2] = index2;

			(*pIndices)[(offset+i)*6+3] = index2;
			(*pIndices)[(offset+i)*6+4] = index1;
			(*pIndices)[(offset+i)*6+5] = index3;
		}
	}

	Geometry* pGeo = WIRE_NEW Geometry(pVertices, pIndices);
	return pGeo;
}

//----------------------------------------------------------------------------
void Sample3::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Matrix34F rotate(Vector3F(1, 0, 0), 1.2F);
	mspRoot->Local.SetRotate(rotate);

 	mspRoot->UpdateGS(time);
 	mCuller.ComputeVisibleSet(mspRoot);

	mpRenderer->ClearBuffers();
	mpRenderer->PreDraw(mspCamera);
	mpRenderer->DrawScene(mCuller.GetVisibleSet());
	mpRenderer->PostDraw();
	mpRenderer->DisplayBackBuffer();
}
