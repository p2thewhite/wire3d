#include "Sample6.h"
#include "LensflareNode.h"

using namespace Wire;

WIRE_APPLICATION(Sample6);

//----------------------------------------------------------------------------
Sample6::Sample6()
{
}

//----------------------------------------------------------------------------
Bool Sample6::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspRoot = WIRE_NEW Node;
	LensflareNode* pLensflare = WIRE_NEW LensflareNode;
	mspRoot->AttachChild(pLensflare);

	Geometry* pSphere = StandardMesh::CreateSphere(20, 20, 1);
	StateWireframe* pWireframe = WIRE_NEW StateWireframe;
	pWireframe->Enabled = true;
	pSphere->AttachState(pWireframe);
	mspRoot->AttachChild(pSphere);
	mspRoot->UpdateRS();

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, 0.0F, 4.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);
	mCuller.SetCamera(mspCamera);

	// Initialize working variables used in the render loop (i.e. OnIdle()).
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
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Matrix3F rot(Vector3F(0, 1, 0.5F), mAngle);
//	mspCamera->SetAxes(rot.GetColumn(0), rot.GetColumn(1), rot.GetColumn(2));
	mspRoot->GetChild(1)->Local.SetRotate(rot);

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSet());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}
