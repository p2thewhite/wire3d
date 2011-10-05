// Sample10 - Material Sorting

#include "Sample10.h"

using namespace Wire;

WIRE_APPLICATION(Sample10);		// This macro creates our user application.

//----------------------------------------------------------------------------
Bool Sample10::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspGeo = StandardMesh::CreateCube8(/* number of RGB(A) channels */ 4);

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, 0.0F, 10.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	UInt width = GetRenderer()->GetWidth();
	UInt height = GetRenderer()->GetHeight();
	Float fieldOfView = 45.0F;
	Float aspectRatio = static_cast<Float>(width)/static_cast<Float>(height);
	Float nearPlane = 0.1F;
	Float farPlane = 300.0F;
	mspCamera->SetFrustum(fieldOfView, aspectRatio, nearPlane, farPlane);
	mCuller.SetCamera(mspCamera);

	return true;
}

//----------------------------------------------------------------------------
void Sample10::OnIdle()
{
	Matrix34F rotate(Vector3F(0.2F, 0.7F, 0.1F),
		MathF::FMod(static_cast<Float>(System::GetTime()), MathF::TWO_PI));
	mspGeo->World.SetRotate(rotate);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->Draw(mspGeo);
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}
