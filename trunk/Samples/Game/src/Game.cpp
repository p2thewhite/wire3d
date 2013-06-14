#include "Game.h"

#include "Importer.h"

WIRE_APPLICATION(Game);

using namespace Wire;

//----------------------------------------------------------------------------
Game::Game() 
 	:
// 	WIREAPPLICATION(
// 		ColorRGBA(0.0F, 0.0F, 0.0F, 1.0F),	// background color
// 		// The following parameters are PC only:
// 		"Game",	// title of the window,
// 		0, 0,		// window position
// 		640, 480,	// window size; (use (0,0) for current desktop resolution)
// 		false,
// 		false),		// fullscreen
	mCursorPosition(Vector2F::ZERO),
	mShowColliders(false),
	mShowFps(false),
	mWasButton1Pressed(false),
	mWasButton2Pressed(false)
{
}

//----------------------------------------------------------------------------
Bool Game::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	InitializePhysics();
	mspLogo = LoadAndInitializeLoading();

	if (!mspLogo)
	{
		WIRE_ASSERT(false /* Could not load logo.xml */);
		return false;
	}

	mAppState = AS_LOADING;
	mLastApplicationTime = System::GetTime();

	// Font for render statistics debug text
	mspText = Importer::CreateText("Data/Logo/font.ttf", 12, 12);
	WIRE_ASSERT(mspText);
	GetRenderer()->Bind(mspText);

	GetRenderer()->CreateBatchingBuffers(50*1024, 50*1024);
	GetRenderer()->SetDynamicBatchingThreshold(300, 100);
	GetRenderer()->SetStaticBatchingThreshold(700);

	return true;
}

//----------------------------------------------------------------------------
void Game::OnTerminate()
{
	mspPhysicsWorld = NULL;
}

//----------------------------------------------------------------------------
void Game::OnIdle()
{
	Double appTime = System::GetTime();
	Double deltaTime = appTime - mLastApplicationTime;
	mLastApplicationTime = appTime;

	mspLogoCamera->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);

	switch (mAppState)
	{
	case AS_LOADING:
		OnLoading(appTime, deltaTime);
		break;

	case AS_RUNNING:
		OnRunning(appTime, deltaTime);
		break;

	default:
		WIRE_ASSERT(false /* Invalid application state */);
	}
}

//----------------------------------------------------------------------------
void Game::ProcessInput()
{
	if (GetInputSystem()->GetMainDevicesCount() == 0)
	{
		return;
	}

	const MainInputDevice* pInputDevice = GetInputSystem()->GetMainDevice(0);

	// Checking minimum capabilities
	if (!pInputDevice->HasCapability(AnalogPad::TYPE, true) && !pInputDevice->
		HasCapability(DigitalPad::TYPE, true))
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

	// Processing IR
	//
	const IR* pIR = DynamicCast<const IR>(pInputDevice->GetCapability(IR::TYPE, false));
	WIRE_ASSERT(pIR);

	Vector2F cursorPosition(pIR->GetRight(), pIR->GetUp());

	// If the IR is pointing outside of the capture area, set the lookAt vector to the previous captured position
	// TODO: fix inconsistent Win32/Wii behavior/handling
	if (cursorPosition.X() == MathF::MAX_REAL || cursorPosition.Y() == MathF::MAX_REAL)
	{
		cursorPosition = mCursorPosition;
	}
	else
	{
		// Height correction
		cursorPosition.Y() = GetHeightF() - cursorPosition.Y();
	}

	mCursorPosition = cursorPosition;
	if (mspCrosshair)
	{
		mspCrosshair->Local.SetTranslate(Vector3F(cursorPosition.X() - 16,
			cursorPosition.Y() - 16, 0));
	}

	// Converting from (top, left) to (horizontal screen center, vertical screen center) coordinate system
	cursorPosition.X() -= GetWidthF() * 0.5F;
	cursorPosition.Y() -= GetHeightF() * 0.5F;

	if (mspPlayer)
	{
		mspPlayer->LookAt(cursorPosition);
	}

	// Processing buttons/keys
	//
	const Buttons* pButtons = DynamicCast<const Buttons>(pInputDevice->
		GetCapability(Buttons::TYPE, false));
	WIRE_ASSERT(pButtons);

	// '1' button/key toggles display of renderer statistics
	if (pButtons->GetButton(Buttons::BUTTON_1))
	{
		if (!mWasButton1Pressed)
		{
			mShowFps = !mShowFps;
			mWasButton1Pressed = true;
		}
	}
	else
	{
		mWasButton1Pressed = false;
	}

	// '2' button/key toggles debug draw of physics collision shapes
	if (pButtons->GetButton(Buttons::BUTTON_2))
	{
		if (!mWasButton2Pressed)
		{
			mShowColliders = !mShowColliders;
			mspPhysicsWorld->ToggleDebugShapes(mShowColliders);
			mWasButton2Pressed = true;
		}
	}
	else
	{
		mWasButton2Pressed = false;
	}
}

#include "RigidBodyController.h"

//----------------------------------------------------------------------------
void Game::OnRunning(Double time, Double deltaTime)
{
	ProcessInput();

// 	Node* pPlatform = (Node*)mspScene->FindChildByName("Platform");
// 
// 	TArray<RigidBodyController*> rbcs;
// 	pPlatform->FindControllers<RigidBodyController>(rbcs);
// 
// 	RigidBodyController* pRBC =	pPlatform->FindController<RigidBodyController>();
// 	btRigidBody* pRB = pRBC->Get();
// 
// 	btTransform trans;
// 	pRB->getMotionState()->getWorldTransform(trans);
// 	btVector3 origin = trans.getOrigin();
// 	static Float yCoord = origin.y();
// 	Float angle = MathF::Sin(time) *3 + 3;
// 	origin[1] = yCoord + angle;
// 	trans.setOrigin(origin);
// 	pRB->getMotionState()->setWorldTransform(trans);

	mspPhysicsWorld->StepSimulation(deltaTime, 10);

	mspScene->UpdateGS(time);
	mSortingCuller.ComputeVisibleSet(mspScene);

	mspGUI->UpdateGS(time);
	mspGUICamera->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);
	mCuller.SetCamera(mspGUICamera);
	mCuller.ComputeVisibleSet(mspGUI);

	GetRenderer()->GetStatistics()->Reset();
	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspSceneCamera);
	GetRenderer()->Draw(mSortingCuller.GetVisibleSets());

	GetRenderer()->SetCamera(mspGUICamera);
	GetRenderer()->Draw(mCuller.GetVisibleSets());

	if (mShowFps)
	{
		GetRenderer()->GetStatistics()->Draw(mspText, Transformation::IDENTITY,
			static_cast<Float>(1.0 / deltaTime));
	}

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void Game::OnLoading(Double time, Double deltaTime)
{
	Spatial* pLoading = mspLogo->FindChildByName("Loading");
	Bool isFadedIn = false;

	if (pLoading)
	{
		StateMaterial* pMaterialState = DynamicCast<StateMaterial>(pLoading->
			GetState(State::MATERIAL));
		if (pMaterialState)
		{
			pMaterialState->Ambient.A() += static_cast<Float>(deltaTime);

			if (pMaterialState->Ambient.A() > 1.0F)
			{
				pMaterialState->Ambient.A() = 1.0F;
				isFadedIn = true;
			}
		}
	}

	mspLogo->UpdateGS(time);

	mCuller.SetCamera(mspLogoCamera);
	mCuller.ComputeVisibleSet(mspLogo);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspLogoCamera);
	GetRenderer()->Draw(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();

	if (!isFadedIn)
	{
		return;
	}

	mspGUI = LoadAndInitializeGUI();
	if (!mspGUI)
	{
		WIRE_ASSERT(false /* Could not load GUI.xml */);
	}

	mspScene = LoadAndInitializeScene();
	if (!mspScene)
	{
		WIRE_ASSERT(false /* Could not load scene.xml */);
	}

	mAppState = AS_RUNNING;
	pLoading->Culling = Spatial::CULL_ALWAYS;

	mspLogo = NULL; // discard Logo scene graph to free memory
}

//----------------------------------------------------------------------------
Node* Game::LoadAndInitializeLoading()
{
	Importer importer("Data/Logo/");
	TArray<CameraPtr> cameras;
	Node* pRoot = importer.LoadSceneFromXml("logo.xml", &cameras);
	if (!pRoot)
	{
		return NULL;
	}

	WIRE_ASSERT(cameras.GetQuantity() > 0 /* No Camera in logo.xml */);
	mspLogoCamera = cameras[0];

	// center 512x256 logo on screen
	pRoot->Local.SetTranslate(Vector3F((GetWidthF()-512.0F) * 0.5F,
		(GetHeightF() - 256.0F)  * 0.5F, 0));

	pRoot->Bind(GetRenderer());
	return pRoot;
}

//----------------------------------------------------------------------------
Node* Game::LoadAndInitializeGUI()
{
	Importer importer("Data/GUI/");
	TArray<CameraPtr> cameras;
	Node* pRoot = importer.LoadSceneFromXml("GUI.xml", &cameras);
	if (!pRoot)
	{
		return NULL;
	}

	WIRE_ASSERT(cameras.GetQuantity() > 0 /* No Camera in GUI.xml */);
	mspGUICamera = cameras[0];

	mspCrosshair = pRoot->FindChildByName("Crosshair");
	WIRE_ASSERT(mspCrosshair /* No Crosshair in GUI.xml */);

	pRoot->Bind(GetRenderer());

	return pRoot;
}

//----------------------------------------------------------------------------
Node* Game::LoadAndInitializeScene()
{
	Importer importer("Data/Scene/");
	TArray<CameraPtr> cameras;
	Node* pScene = importer.LoadSceneFromXml("Scene.xml", &cameras,
		mspPhysicsWorld);
	if (!pScene)
	{
		return NULL;
	}

	WIRE_ASSERT(cameras.GetQuantity() > 0 /* No Camera in scene.xml */);
	mspSceneCamera = cameras[0];
	mspSceneCamera->SetAspectRatio(GetWidthF() / GetHeightF());
	mSortingCuller.SetCamera(mspSceneCamera);

	// The maximum number of objects that are going to be culled is the
	// number of objects we imported. If we don't set the size of the set now,
	// the culler will dynamically increase it during runtime. This is not
	// a big deal, however it is better to avoid memory allocations during the
	// render loop.
	UInt renderObjectCount = importer.GetStatistics()->RenderObjectCount;
	mSortingCuller.SetMaxQuantity(renderObjectCount);

	Spatial* pProbeRobotSpatial = pScene->FindChildByName("Probe Robot");
	Spatial* pPlayerSpatial = pScene->FindChildByName("Player");

	// Create and configure probe robot controller
	Spatial* pRedHealthBar = mspGUI->FindChildByName("RedHealthBar");
	WIRE_ASSERT(pRedHealthBar /* No RedHealthBar in GUI.xml */);
	pRedHealthBar->Local.SetTranslate(Vector3F(276, GetHeightF() - 26.0F, 0));
	mspGUI->UpdateGS();

	mspProbeRobot = WIRE_NEW ProbeRobot(pPlayerSpatial, pRedHealthBar);
	pProbeRobotSpatial->AttachController(mspProbeRobot);
	mspProbeRobot->Register(mspPhysicsWorld->Get());

	// Create and configure player controller
	mspPlayer = WIRE_NEW Player(mspSceneCamera);
	pPlayerSpatial->AttachController(mspPlayer);
	mspPlayer->Register(mspPhysicsWorld);

	pScene->Bind(GetRenderer());
	pScene->WarmUpRendering(GetRenderer());

	return pScene;
}

//----------------------------------------------------------------------------
void Game::InitializePhysics()
{
	mspPhysicsWorld = WIRE_NEW PhysicsWorld(Vector3F(-120, -120, -120),
		Vector3F(120, 120, 120));

	mspPhysicsWorld->Get()->getPairCache()->setInternalGhostPairCallback(
		WIRE_NEW btGhostPairCallback());
}
