#include "Demo.h"

#include "Controllers/ConveyorBelt.h"
#include "Controllers/FanRotator.h"
#include "Controllers/LogoFader.h"
#include "Importer.h"
#include "WireInputSystem.h"
#include "WireDigitalPad.h"
#include "WireIR.h"
#include "WireButtons.h"

using namespace Wire;

WIRE_APPLICATION(Demo);

//----------------------------------------------------------------------------
Demo::Demo() :
	mpFirstPersonController(NULL)
{
}

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
		WIRE_ASSERT(false /* Could not load logo.xml */);
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

	UpdateCameraFrustumAccordingToScreenDimensions(mLogoCameras[0]);

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
	if (mpFirstPersonController == NULL)
	{
		return;
	}

	if (GetInputSystem()->GetMainDevicesCount() == 0)
	{
		return;
	}

	const MainInputDevice* pInputDevice = GetInputSystem()->GetMainDevice(0);
	// checking for minimum capabilities
	if (!pInputDevice->HasCapability(DigitalPad::TYPE, true))
	{
		return;
	}

	if (!pInputDevice->HasCapability(IR::TYPE, true))
	{
		return;
	}

	if (!pInputDevice->HasCapability(Buttons::TYPE, true))
	{
		return;
	}

	// ---
	const DigitalPad* pDigitalPad = static_cast<const DigitalPad*>(pInputDevice->GetCapability(DigitalPad::TYPE, false));
	if (pDigitalPad->GetUp())
	{
		mpFirstPersonController->MoveForward();
	} 
	else if (pDigitalPad->GetDown())
	{
		mpFirstPersonController->MoveBackward();
	}
	else if (pDigitalPad->GetLeft())
	{
		mpFirstPersonController->StrafeLeft();
	}
	else if (pDigitalPad->GetRight())
	{
		mpFirstPersonController->StrafeRight();
	}

	// ---
	const IR* pIR = static_cast<const IR*>(pInputDevice->GetCapability(IR::TYPE, false));
	Float screenWidth = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float x = pIR->GetRight();
	Float y = screenHeight - pIR->GetUp();

	MoveCrosshairTo(Vector2F(x, y));

	// converting from (top, left) to (horizontal screen center, vertical screen center) 
	// coordinate system
	x -= screenWidth * 0.5F;
	y -= screenHeight * 0.5F;

	mpFirstPersonController->LookUp(Vector2F(x, y));

	// ---
	const Buttons* pButtons = static_cast<const Buttons*>(pInputDevice->GetCapability(Buttons::TYPE, false));

	// when home is pressed, exit the game
	if (pButtons->GetButton(Buttons::BUTTON_HOME))
	{
		Close();
		return;
	}

	// b button makes the player run faster
	if (pButtons->GetButton(Buttons::BUTTON_B))
	{
		mpFirstPersonController->SetMoveSpeed(5.0f);
	}
	else 
	{
		mpFirstPersonController->SetMoveSpeed(2.5f);
	}
}

//----------------------------------------------------------------------------
void Demo::StateRunning(Double time)
{
	UpdateCameraFrustumAccordingToScreenDimensions(mGUICameras[0]);

	mspLogo->UpdateGS(time);
	mLogoCuller.ComputeVisibleSet(mspLogo);

	mspGUI->UpdateGS(time);
	mGUICuller.ComputeVisibleSet(mspGUI);

	mspScene->UpdateGS(time);
	mSceneCuller.ComputeVisibleSet(mspScene);

	GetRenderer()->ResetStatistics();

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mSceneCameras[0]);
	GetRenderer()->DrawScene(mSceneCuller.GetVisibleSets());

	GetRenderer()->SetCamera(mGUICameras[0]);
	GetRenderer()->DrawScene(mGUICuller.GetVisibleSets());

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
	static Double lastTime = 0.0f;
	Double elapsed = time - lastTime;
	lastTime = time;

	// set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
	mspTextCamera->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);
	GetRenderer()->SetCamera(mspTextCamera);

	// set to screen screenWidth (might change any time in window mode)
	mspText->SetLineWidth(screenWidth);
	mspText->Clear(Color32::WHITE);
	// Text uses OpenGL convention of (0,0) being left bottom of window
	mspText->SetPen(0, screenHeight-mspText->GetFontHeight()-32.0F);

	const UInt TextArraySize = 1000;
	Char text[TextArraySize];
	UInt fps = static_cast<UInt>(1/elapsed);
	String msg1 = "\nFPS: %d\nDraw Calls: %d, Triangles: %d\nBatched Static: "
		"%d, Batched Dynamic: %d\nVBOs: %d, VBOSize: %.2f KB\nIBOs: %d, "
		"IBOSize: %.2f KB\nTextures: %d, TextureSize: %.2f MB";
	Float kb = 1024.0F;
	const Renderer::Statistics* pStats = Renderer::GetStatistics();
	System::Sprintf(text, TextArraySize, static_cast<const Char*>(msg1), fps,
		pStats->DrawCalls, pStats->Triangles, pStats->BatchedStatic,
		pStats->BatchedDynamic, pStats->VBOCount, pStats->VBOTotalSize/kb,
		pStats->IBOCount, pStats->IBOTotalSize/kb, pStats->TextureCount,
		pStats->TextureTotalSize/(kb*kb));
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
		StateMaterial* pMaterialState = static_cast<StateMaterial*>(pLoading->
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
		WIRE_ASSERT(false /* Could not load scene.xml */);
	}

	mspGUI = LoadAndInitGUI();
	if (!mspGUI)
	{
		WIRE_ASSERT(false /* Could not load GUI.xml */);
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

	WIRE_ASSERT(mLogoCameras.GetQuantity() > 0 /* No Camera in logo.xml */);
	mLogoCuller.SetCamera(mLogoCameras[0]);

	Spatial* pLogo = pRoot->GetChildByName("Logo");
	WIRE_ASSERT(pLogo != NULL);

	if (pLogo)
	{
		pLogo->AttachController(WIRE_NEW LogoFader);
	}

	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
	pRoot->Local.SetTranslate(Vector3F((screenWidth - 512) * 0.5F, (screenHeight  -256)  *0.5F, 0));

	GetRenderer()->BindAll(pRoot);
	return pRoot;
}

//----------------------------------------------------------------------------
Node* Demo::LoadAndInitGUI()
{
	Importer importer("Data/GUI/");
	Node* pRoot = importer.LoadSceneFromXml("GUI.xml", &mGUICameras);
	if (!pRoot)
	{
		return NULL;
	}

	WIRE_ASSERT(mGUICameras.GetQuantity() > 0 /* No Camera in GUI.xml */);
	mGUICuller.SetCamera(mGUICameras[0]);

	mspCrosshair = pRoot->GetChildByName("Crosshair");
	WIRE_ASSERT(mspCrosshair != NULL);

	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());

	Geometry* pMainInputDeviceIcon = static_cast<Geometry*>(pRoot->GetChildByName("MainInputDeviceIcon"));
	WIRE_ASSERT(pMainInputDeviceIcon != NULL);

	pMainInputDeviceIcon->Local.SetTranslate(Vector3F(0, screenHeight - 64, 0));

	// loading the main input device icon dynamically according to the platform
#ifdef WIRE_WII
	pMainInputDeviceIcon->GetMaterial()->SetTexture(0, CreateTexture(importer.LoadPNG("Data/GUI/wiiMoteIcon.png", false)));
#else
	pMainInputDeviceIcon->GetMaterial()->SetTexture(0, CreateTexture(importer.LoadPNG("Data/GUI/keyboardIcon.png", false)));
#endif
	
	Geometry* pInputDeviceExtensionIcon = static_cast<Geometry*>(pRoot->GetChildByName("InputDeviceExtensionIcon"));
	WIRE_ASSERT(pMainInputDeviceIcon != NULL);

	pInputDeviceExtensionIcon->Local.SetTranslate(Vector3F(64, screenHeight - 64, 0));

	// loading the input device extension icon dynamically according to the platform
#ifdef WIRE_WII
	pInputDeviceExtensionIcon->GetMaterial()->SetTexture(0, CreateTexture(importer.LoadPNG("Data/GUI/nunchukIcon.png", false)));
#else
	pInputDeviceExtensionIcon->GetMaterial()->SetTexture(0, CreateTexture(importer.LoadPNG("Data/GUI/mouseIcon.png", false)));
#endif

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

	WIRE_ASSERT(mSceneCameras.GetQuantity() > 0 /* No Camera in scene.xml */);

	Float fov, aspect, near, far;
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	mSceneCameras[0]->GetFrustum(fov, aspect, near, far);
	aspect = screenWidth / screenHeight;
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

	mpFirstPersonController = WIRE_NEW FirstPersonController(Vector3F(13.0f, 1.7f, -21.0f), 
		mSceneCameras[0]);

	mpFirstPersonController->SetLookUpDeadZone(Vector2F(50, 50));

	pScene->AttachController(mpFirstPersonController);

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

//----------------------------------------------------------------------------
void Demo::UpdateCameraFrustumAccordingToScreenDimensions(Camera* pCamera)
{
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
	pCamera->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);
}

//----------------------------------------------------------------------------
Texture2D* Demo::CreateTexture(Image2D* pImage)
{
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	pTexture->SetFilterType(Texture2D::FT_LINEAR_NEAREST);
	pTexture->SetWrapType(0, Texture2D::WT_CLAMP);
	pTexture->SetWrapType(1, Texture2D::WT_CLAMP);
	pTexture->SetAnisotropyValue(1);
	return pTexture;
}

//----------------------------------------------------------------------------
void Demo::MoveCrosshairTo(const Vector2F& rScreenPosition)
{
	mspCrosshair->Local.SetTranslate(Vector3F(rScreenPosition.X() - 16, rScreenPosition.Y() - 16, 0));
}
