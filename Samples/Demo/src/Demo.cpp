#include "Demo.h"

#include "Controllers/ConveyorBelt.h"
#include "Controllers/FanRotator.h"
#include "Controllers/LogoFader.h"
#include "Controllers/SplineCamera.h"
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

	mspLogo = LoadAndInitLogo();
	if (!mspLogo)
	{
		WIRE_ASSERT(false /* Could not load Logo.xml */);
		return false;
	}

	mAppState = AS_LOADING;
	mLastTime = System::GetTime();
	mShowFps = false;

	// frames per second and render statistics debug text
	mspTextCamera = WIRE_NEW Camera(/* isPerspective */ false);
	mspText = Importer::CreateText("Data/Logo/cour.ttf", 20, 20);
	WIRE_ASSERT(mspText);
	GetRenderer()->BindAll(mspText);

 	GetRenderer()->CreateBatchingBuffers(60*1024);
 	GetRenderer()->SetDynamicBatchingThreshold(300);
 	GetRenderer()->SetStaticBatchingThreshold(10000);

	return true;
}

//----------------------------------------------------------------------------
void Demo::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mLogoCameras[0]->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);
	Vector3F centered((GetWidthF()-512)*0.5F, (GetHeightF()-256)*0.5F, 0);
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
void Demo::OnInput()
{
	if (GetInputSystem()->GetMainDevicesCount() == 0)
	{
		return;
	}

	const MainInputDevice* pInputDevice = GetInputSystem()->GetMainDevice(0);

	// checking for minimum capabilities
	if (!pInputDevice->HasCapability(Buttons::TYPE, true))
	{
		return;
	}

	const Buttons* pButtons = DynamicCast<Buttons>(pInputDevice->
		GetCapability(Buttons::TYPE, false));
	WIRE_ASSERT(pButtons);

	// when home is pressed, exit the game
	if (pButtons->GetButton(Buttons::BUTTON_HOME))
	{
		Close();
		return;
	}

	mShowFps = pButtons->GetButton(Buttons::BUTTON_B);
}

//----------------------------------------------------------------------------
void Demo::StateRunning(Double time)
{
	mspLogo->UpdateGS(time);
	mLogoCuller.ComputeVisibleSet(mspLogo);

	mspScene->UpdateGS(time);
	mSceneCuller.ComputeVisibleSet(mspScene);

	GetRenderer()->GetStatistics()->Reset();

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mSceneCameras[0]);
	GetRenderer()->DrawScene(mSceneCuller.GetVisibleSets());

	GetRenderer()->SetCamera(mLogoCameras[0]);
	GetRenderer()->DrawScene(mLogoCuller.GetVisibleSets());

	if (mShowFps)
	{
		DrawFPS(time);
	}

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void Demo::DrawFPS(Double time)
{
	static Double lastTime = 0.0F;
	Double elapsed = time - lastTime;
	lastTime = time;

	// set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	mspTextCamera->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);
	GetRenderer()->SetCamera(mspTextCamera);

	// set to screen width (might change any time in window mode)
	mspText->SetLineWidth(GetWidthF());
	mspText->Clear(Color32::WHITE);
	// Text uses OpenGL convention of (0,0) being left bottom of window
	mspText->SetPen(0, GetHeightF()-mspText->GetFontHeight()-32.0F);

	const UInt TextArraySize = 1000;
	Char text[TextArraySize];
	UInt fps = static_cast<UInt>(1/elapsed);
	String msg1 = "\nFPS: %d\nDraw Calls: %d, Triangles: %d\nBatched Static: "
		"%d, Batched Dynamic: %d\nVBOs: %d, VBOSize: %.2f KB\nIBOs: %d, "
		"IBOSize: %.2f KB\nTextures: %d, TextureSize: %.2f MB";
	Float kb = 1024.0F;
	const RendererStatistics* pStats = GetRenderer()->GetStatistics();
	System::Sprintf(text, TextArraySize, static_cast<const Char*>(msg1), fps,
		pStats->GetDrawCalls(), pStats->GetTriangles(), pStats->
		GetBatchedStatic(), pStats->GetBatchedDynamic(), pStats->
		GetVBOCount(), pStats->GetVBOTotalSize()/kb, pStats->GetIBOCount(),
		pStats->GetIBOTotalSize()/kb, pStats->GetTextureCount(), pStats->
		GetTextureTotalSize()/(kb*kb));
	mspText->SetColor(Color32::WHITE);
	mspText->Append(text);

	mspText->Update(GetRenderer());
	GetRenderer()->DisableLighting();
	GetRenderer()->Draw(mspText);
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
			pMaterialState->Ambient.A() += static_cast<Float>(
				elapsedTime) * 0.5F;

			if (pMaterialState->Ambient.A() > 1.0F)
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

	mspScene = LoadAndInitScene();
	if (!mspScene)
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
Node* Demo::LoadAndInitScene()
{
	Importer importer("Data/");
	Node* pScene = importer.LoadSceneFromXml("scene.xml", &mSceneCameras);
	if (!pScene)
	{
		return NULL;
	}

	WIRE_ASSERT(mSceneCameras.GetQuantity() > 0 /* No Camera in Scene.xml */);
	Float fov, aspect, near, far;
	mSceneCameras[0]->GetFrustum(fov, aspect, near, far);
	aspect = GetWidthF() / GetHeightF();
	mSceneCameras[0]->SetFrustum(fov, aspect, near, far);
	mSceneCuller.SetCamera(mSceneCameras[0]);

	// The maximum number of objects that are going to be culled is the
	// number of objects we imported. If we don't set the size of the set now,
	// the culler will dynamically increase it during runtime. This is not
	// a big deal, however we do not want any memory allocations during the
	// render loop.
	UInt geometryCount = importer.GetStatistics()->GeometryCount;
	for (UInt i = 0; i < mSceneCuller.GetVisibleSets().GetQuantity(); i++)
	{
		mSceneCuller.GetVisibleSet(i)->SetMaxQuantity(geometryCount);
	}

	TArray<Spatial*> fans;
	pScene->GetAllChildrenByName("ceilingFan", fans);
	for (UInt i = 0; i < fans.GetQuantity(); i++)
	{
		Float f = static_cast<Float>(i+1);
		fans[i]->AttachController(WIRE_NEW FanRotator(f));
	}

	Node* pSplineRoot = DynamicCast<Node>(pScene->GetChildByName("Spline"));
	pScene->AttachController(WIRE_NEW SplineCamera(pSplineRoot,
		mSceneCameras[0]));

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
