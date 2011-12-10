#include "Demo.h"
#include "Importer.h"

using namespace Wire;

WIRE_APPLICATION(Demo);

//----------------------------------------------------------------------------
Bool Demo::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	Importer importer("Data/");
	mspRoot = importer.LoadSceneFromXml("InnsbruckMobile.xml");

	// Once we finished creating the scene graph, we update the graph's
	// render states. This propagates the fog state to all child nodes.
	// Whenever you change the graph's render state (using Attach-/
	// DetachState() on any of its children), you must call UpdateRS().
	mspRoot->UpdateRS();

	// setup our camera at the origin, looking down the -z axis with y up
	Vector3F cameraLocation(0.0F, 0.0F, 0.0F);
	Vector3F viewDirection(0.0F, 0.0F, 1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// specify FOV, aspect ratio, near and far plane of the frustum
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(60, width / height , 0.1F, 30000.0F);

	// the culler needs to know which camera to use when performing its task
	mCuller.SetCamera(mspCamera);

	mAngle = 0.0F;
	mLastTime = System::GetTime();
	return true;
}

//----------------------------------------------------------------------------
void Demo::OnIdle()
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

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}
