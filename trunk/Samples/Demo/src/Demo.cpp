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
	mspRoot = importer.LoadSceneFromXml("scene.xml", &mCameras);
	if (!mspRoot)
	{
		return false;
	}

	Node* pSplineRoot = DynamicCast<Node>(mspRoot->GetChildByName("Spline"));
	if (!pSplineRoot)
	{
		return false;
	}

	for (UInt i = 0; i < pSplineRoot->GetQuantity(); i++)
	{
		Spatial* pChild = pSplineRoot->GetChild(i);
		if (pChild)
		{
			mSplinePoints.Append(&(pChild->Local));
		}
	}

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

	mT = 0;
	mSplinePointIndex = 1;

	return true;
}

//----------------------------------------------------------------------------
void Demo::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	Vector3F camPos = GetHermite(mSplinePoints, mSplinePointIndex, mT);
	mT += static_cast<Float>(elapsedTime*0.25f);
	if (mT > 1.0f)
	{
		mT = 0.0f;
		mSplinePointIndex++;
		if (mSplinePointIndex == mSplinePoints.GetQuantity()-3)
		{
			mSplinePointIndex = 1;
		}
	}

	mspCamera->SetLocation(camPos);

	mspRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Vector3F Demo::GetHermite(TArray<Transformation*>& rControlPoints, UInt idx,
	Float t) const
{
	WIRE_ASSERT(idx > 0 && idx < rControlPoints.GetQuantity()-3);
	Float t2 = t * t;
	Float t3 = t2 * t;

	Vector3F& rP0 = (*(rControlPoints[idx - 1])).GetTranslate();
	Vector3F& rP1 = (*(rControlPoints[idx])).GetTranslate();
	Vector3F& rP2 = (*(rControlPoints[idx + 1])).GetTranslate();
	Vector3F& rP3 = (*(rControlPoints[idx + 2])).GetTranslate();

	const Float tension = 0.5f;	// 0.5 is catmull-rom

	Vector3F v4 = tension * (rP2 - rP0);
	Vector3F v5 = tension * (rP3 - rP1);

	Float blend1 = 2 * t3 - 3 * t2 + 1;
	Float blend2 = -2 * t3 + 3 * t2;
	Float blend3 = t3 - 2 * t2 + t;
	Float blend4 = t3 - t2;

	return blend1 * rP1 + blend2 * rP2 + blend3 * v4 + blend4 * v5;
}
