#include "Demo.h"

#include "ConveyorBelt.h"
#include "FanRotator.h"
#include "Importer.h"
#include "SplineCamera.h"

using namespace Wire;

WIRE_APPLICATION(Demo);

//----------------------------------------------------------------------------
Bool Demo::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspScene1 = LoadAndInitScene1();
	if (!mspScene1)
	{
		return false;
	}

	if (!mspCamera)
	{
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
	}

	mLastTime = System::GetTime();
	return true;
}

//----------------------------------------------------------------------------
void Demo::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mspScene1->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspScene1);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Node* Demo::LoadAndInitScene1()
{
	Importer importer("Data/");
	Node* pScene = importer.LoadSceneFromXml("scene.xml", &mCameras);
	if (!pScene)
	{
		return NULL;
	}

	if (mCameras.GetQuantity() > 0)
	{
		Float fov, aspect, near, far;
		Float width = static_cast<Float>(GetRenderer()->GetWidth());
		Float height = static_cast<Float>(GetRenderer()->GetHeight());
		mspCamera = mCameras[0];
		mspCamera->GetFrustum(fov, aspect, near, far);
		aspect = width / height;
		mspCamera->SetFrustum(fov, aspect, near, far);
		mCuller.SetCamera(mspCamera);
	}

	TArray<Spatial*> fans;
	pScene->GetAllChildrenByName("ceilingFan", fans);
	for (UInt i = 0; i < fans.GetQuantity(); i++)
	{
		Float f = static_cast<Float>(i+1);
		fans[i]->AttachController(WIRE_NEW FanRotator(f));
	}

	pScene->UpdateRS();

	Node* pSplineRoot = DynamicCast<Node>(pScene->GetChildByName("Spline"));
	pScene->AttachController(WIRE_NEW SplineCamera(pSplineRoot, mspCamera));

	Geometry* pConveyorBelt = DynamicCast<Geometry>(pScene->GetChildByName(
		"polySurface437"));
	if (pConveyorBelt)
	{
		pConveyorBelt->AttachController(WIRE_NEW ConveyorBelt(pConveyorBelt,
			GetRenderer()));
	}	

	GetRenderer()->BindAll(pScene);
	return pScene;
}
