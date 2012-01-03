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

	mspLogo = LoadAndInitLogo();
	if (!mspLogo)
	{
		WIRE_ASSERT(false /* Could not load Logo.xml */);
		return false;
	}

	mspScene1 = LoadAndInitScene1();
	if (!mspScene1)
	{
		WIRE_ASSERT(false /* Could not load Scene.xml */);
		return false;
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

	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
//	mLogoCameras[0]->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);
	mspLogo->UpdateGS(time);
	mLogoCuller.SetCamera(mLogoCameras[0]);
	mLogoCuller.ComputeVisibleSet(mspLogo);

	mspScene1->UpdateGS(time);
	mScene1Culler.ComputeVisibleSet(mspScene1);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mScene1Cameras[0]);
	GetRenderer()->DrawScene(mScene1Culler.GetVisibleSets());

	GetRenderer()->SetCamera(mLogoCameras[0]);
	GetRenderer()->DrawScene(mLogoCuller.GetVisibleSets());

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Node* Demo::LoadAndInitLogo()
{
	Importer importer("Data/Logo/");
	Node* pLogo = importer.LoadSceneFromXml("logo.xml", &mLogoCameras);
	if (!pLogo)
	{
		return NULL;
	}

	WIRE_ASSERT(mLogoCameras.GetQuantity() > 0 /* No Camera in Logo.xml */);

	StateAlpha* pAlpha = WIRE_NEW StateAlpha;
	pAlpha->BlendEnabled = true;
	pLogo->AttachState(pAlpha);

	StateZBuffer* pZBuffer = WIRE_NEW StateZBuffer;
	pZBuffer->Writable = false;
	pZBuffer->Enabled = false;
	pLogo->AttachState(pZBuffer);

	Light* pLight = WIRE_NEW Light();
	pLight->Ambient = ColorRGB::WHITE;
	pLogo->AttachLight(pLight);

	StateMaterial* pStateMaterial = WIRE_NEW StateMaterial;
	pStateMaterial->Ambient = ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	pLogo->AttachState(pStateMaterial);

	pLogo->UpdateRS();

	GetRenderer()->BindAll(pLogo);
	return pLogo;
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
