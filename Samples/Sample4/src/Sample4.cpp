#include "Sample4.h"
#include "LensflareNode.h"

using namespace Wire;

WIRE_APPLICATION(Sample4);

//----------------------------------------------------------------------------
Sample4::Sample4()
{
}

//----------------------------------------------------------------------------
Bool Sample4::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspLensflare = WIRE_NEW LensflareNode;

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
void Sample4::OnIdle()
{
	// This function is called by the framework's render loop until the
	// application exits. Perform all your rendering here.

	// Determine how much time has passed since the last call, so we can
	// move our objects independently of the actual frame rate.
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	mspLensflare->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspLensflare);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSet());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}
