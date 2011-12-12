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
	mspRoot = importer.LoadSceneFromXml("TheFinalSceneiPhoneControls.xml", &mCameras);

	mspRoot->UpdateRS();

	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());

	if (mCameras.GetQuantity() > 0)
	{
		Float fov;
		Float aspect;
		Float near;
		Float far;
		mspCamera = mCameras[0];
		mspCamera->GetFrustum(fov, aspect, near, far);
		aspect = width / height;
		mspCamera->SetFrustum(fov, aspect, near, far);
		mCuller.SetCamera(mspCamera);
	}
	else
	{
		// setup our camera at the origin, looking down the -z axis with y up
		Vector3F cameraLocation(0.0F, 0.0F, 0.0F);
		Vector3F viewDirection(0.0F, 0.0F, 1.0F);
		Vector3F up(0.0F, 1.0F, 0.0F);
		Vector3F right = viewDirection.Cross(up);
		mspCamera = WIRE_NEW Camera;
		mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

		// specify FOV, aspect ratio, near and far plane of the frustum
		mspCamera->SetFrustum(60, width / height , 0.1F, 30000.0F);

		// the culler needs to know which camera to use when performing its task
		mCuller.SetCamera(mspCamera);
	}

	GetRenderer()->BindAll(mspRoot);

	mLastTime = System::GetTime();
	mAngle = 0;
	return true;
}

//----------------------------------------------------------------------------
void Demo::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	mAngle += static_cast<Float>(elapsedTime * 0.25F);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Matrix34F model(Vector3F::UNIT_Y, -mAngle);
	mspRoot->Local.SetRotate(model);

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}
