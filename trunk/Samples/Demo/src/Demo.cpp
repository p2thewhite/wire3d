#include "Demo.h"

#include "Controllers/ConveyorBelt.h"
#include "Controllers/FanRotator.h"
#include "Controllers/LogoFader.h"
#include "Controllers/SplineCamera.h"
#include "Importer/Importer.h"

using namespace Wire;

WIRE_APPLICATION(Demo);

//----------------------------------------------------------------------------
Bool Demo::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspLogo = LoadAndInitLogo();
	if (!mspLogo)
	{
		WIRE_ASSERT(false /* Could not load Logo.xml */);
		return false;
	}

	mAppState = AS_LOADING;
	mLastTime = System::GetTime();
	return true;
}

//----------------------------------------------------------------------------
void Demo::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	mLogoCameras[0]->SetFrustum(0, width, 0, height, 0, 1);
	Vector3F centered((width-512)*0.5F, (height-256)*0.5F, 0);
	mspLogo->Local.SetTranslate(centered);

	switch (mAppState)
	{
	case AS_LOADING:
		StateLoading(elapsedTime);
		break;

	case AS_RUNNING:
		StateRunning(time);
		break;

	default:
		WIRE_ASSERT(false /* Invalid application state */);
	}
}

//----------------------------------------------------------------------------
void Demo::StateRunning(Double time)
{
	mspLogo->UpdateGS(time);
	mLogoCuller.ComputeVisibleSet(mspLogo);

	mspScene1->UpdateGS(time);
	mScene1Culler.ComputeVisibleSet(mspScene1);

	GetRenderer()->ResetStatistics();

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mScene1Cameras[0]);
	GetRenderer()->DrawScene(mScene1Culler.GetVisibleSets());

	GetRenderer()->SetCamera(mLogoCameras[0]);
	GetRenderer()->DrawScene(mLogoCuller.GetVisibleSets());

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void Demo::StateLoading(Double elapsedTime)
{
	Spatial* pLoading = mspLogo->GetChildByName("Loading");
	Bool isFadedIn = false;
	if (pLoading)
	{
		StateMaterial* pMaterialState = DynamicCast<StateMaterial>(pLoading->
			GetState(State::MATERIAL));
		if (pMaterialState)
		{
			if (pMaterialState->Ambient.A() < 1.0F)
			{
				pMaterialState->Ambient.A() += static_cast<Float>(
					elapsedTime) * 0.5F;
			}
			else
			{
				pMaterialState->Ambient.A() = 1.0F;
				isFadedIn = true;	
			}
		}
	}

	mspLogo->UpdateGS();
	mLogoCuller.ComputeVisibleSet(mspLogo);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mLogoCameras[0]);
	GetRenderer()->DrawScene(mLogoCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();

	if (!isFadedIn)
	{
		return;
	}

	mspScene1 = LoadAndInitScene1();
	if (!mspScene1)
	{
		WIRE_ASSERT(false /* Could not load Scene.xml */);
	}

	mAppState = AS_RUNNING;
	pLoading->Culling = Spatial::CULL_ALWAYS;
}

//----------------------------------------------------------------------------
Node* Demo::LoadAndInitLogo()
{
	Importer importer("Data/Logo/");
	Node* pRoot = importer.LoadSceneFromXml("logo.xml", &mLogoCameras);
	if (!pRoot)
	{
		return NULL;
	}

	WIRE_ASSERT(mLogoCameras.GetQuantity() > 0 /* No Camera in Logo.xml */);
	mLogoCuller.SetCamera(mLogoCameras[0]);

	Spatial* pLogo = pRoot->GetChildByName("Logo");
	if (pLogo)
	{
		pLogo->AttachController(WIRE_NEW LogoFader);
	}

	GetRenderer()->BindAll(pRoot);
	return pRoot;
}

//----------------------------------------------------------------------------
Node* Demo::LoadAndInitScene1()
{
	Importer importer("Data/");
	Node* pScene = importer.LoadSceneFromXml("scene.xml", &mScene1Cameras);
	if (!pScene)
	{
		return NULL;
	}

	WIRE_ASSERT(mScene1Cameras.GetQuantity() > 0 /* No Camera in Scene.xml */);
	Float fov, aspect, near, far;
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mScene1Cameras[0]->GetFrustum(fov, aspect, near, far);
	aspect = width / height;
	mScene1Cameras[0]->SetFrustum(fov, aspect, near, far);
	mScene1Culler.SetCamera(mScene1Cameras[0]);

	TArray<Spatial*> fans;
	pScene->GetAllChildrenByName("ceilingFan", fans);
	for (UInt i = 0; i < fans.GetQuantity(); i++)
	{
		Float f = static_cast<Float>(i+1);
		fans[i]->AttachController(WIRE_NEW FanRotator(f));
	}

	Node* pSplineRoot = DynamicCast<Node>(pScene->GetChildByName("Spline"));
	pScene->AttachController(WIRE_NEW SplineCamera(pSplineRoot,
		mScene1Cameras[0]));

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
