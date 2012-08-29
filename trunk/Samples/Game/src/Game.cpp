#include "Game.h"

#include "Importer.h"
#include "CollisionShapeToGeometryConverter.h"

WIRE_APPLICATION(Game);

//----------------------------------------------------------------------------
Game::Game() 
	:
	mShowColliders(false)
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
	mLastTime = System::GetTime();
	mShowFps = false;

	// Frames per second and render statistics debug text
	mspTextCamera = WIRE_NEW Camera(/* isPerspective */ false);
	mspText = Importer::CreateText("Data/Logo/cour.ttf", 20, 20);
	WIRE_ASSERT(mspText);

	GetRenderer()->BindAll(mspText);
	GetRenderer()->CreateBatchingBuffers(100 * 1024);
	GetRenderer()->SetDynamicBatchingThreshold(300);
	GetRenderer()->SetStaticBatchingThreshold(2000);

	return true;
}

//----------------------------------------------------------------------------
void Game::OnTerminate()
{
	TerminatePhysics();
}

//----------------------------------------------------------------------------
void Game::OnIdle()
{
	Double time = System::GetTime();
	Double deltaTime = time - mLastTime;
	mLastTime = time;

	mLogoCameras[0]->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);

	switch (mAppState)
	{
	case AS_LOADING:
		OnLoading(time, deltaTime);
		break;

	case AS_RUNNING:
		OnRunning(time, deltaTime);
		break;

	default:
		WIRE_ASSERT(false /* Invalid application state */);
	}
}

//----------------------------------------------------------------------------
void Game::OnInput()
{
	static Double lastButton1PressTime = 0;
	static Float oldX = 0;
	static Float oldY = 0;
	Double time = System::GetTime();

	if (mspPlayer == NULL)
	{
		return;
	}

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

	// ---
	// Processing analog/digital pad

	if (pInputDevice->HasCapability(AnalogPad::TYPE, true))
	{
		const AnalogPad* pAnalogPad = DynamicCast<const AnalogPad>(pInputDevice->
			GetCapability(AnalogPad::TYPE, true));
		WIRE_ASSERT(pAnalogPad);

		if (pAnalogPad->GetUp() > 0)
		{
			mspPlayer->MoveForward();
		}
		else if (pAnalogPad->GetDown() > 0)
		{
			mspPlayer->MoveBackward();
		}

		if (pAnalogPad->GetRight() > 0)
		{
			mspPlayer->StrafeRight();
		}
		else if (pAnalogPad->GetLeft() > 0)
		{
			mspPlayer->StrafeLeft();
		}
	}
	else 
	{
		const DigitalPad* pDigitalPad = DynamicCast<const DigitalPad>(pInputDevice->
			GetCapability(DigitalPad::TYPE, false));
		WIRE_ASSERT(pDigitalPad);

		if (pDigitalPad->GetUp())
		{
			mspPlayer->MoveForward();
		}
		else if (pDigitalPad->GetDown())
		{
			mspPlayer->MoveBackward();
		}

		if (pDigitalPad->GetLeft())
		{
			mspPlayer->StrafeLeft();
		}
		else if (pDigitalPad->GetRight())
		{
			mspPlayer->StrafeRight();
		}
	}

	// ---
	// Processing IR

	const IR* pIR = DynamicCast<const IR>(pInputDevice->GetCapability(IR::TYPE, false));
	WIRE_ASSERT(pIR);

	Float x = pIR->GetRight();
	Float y = pIR->GetUp();

	// If the IR is pointing outside of the capture area, set the lookAt vector to the previous captured position
	if (x == MathF::MAX_REAL || y == MathF::MAX_REAL)
	{
		x = oldX;
		y = oldY;
	}
	else
	{
		// Height correction
		y = GetHeightF() - y;
	}

	oldX = x;
	oldY = y;

	MoveCrosshairTo(Vector2F(x, y));

	// Converting from (top, left) to (horizontal screen center, vertical screen center) coordinate system
	x -= GetWidthF() * 0.5F;
	y -= GetHeightF() * 0.5F;

	mspPlayer->LookAt(Vector2F(x, y));

	// ---
	// Processing buttons

	const Buttons* pButtons = DynamicCast<const Buttons>(pInputDevice->
		GetCapability(Buttons::TYPE, false));
	WIRE_ASSERT(pButtons);

	// 'HOME' button exit the game
	if (pButtons->GetButton(Buttons::BUTTON_HOME))
	{
		Close();
		return;
	}

	// 'PLUS' button displays renderer statistics
	if (pButtons->GetButton(Buttons::BUTTON_PLUS))
	{
		mShowFps = true;
	}
	else if (pButtons->GetButton(Buttons::BUTTON_MINUS))
	{
		mShowFps = false;
	}

	// 'A' button makes the player shoot
	if (pButtons->GetButton(Buttons::BUTTON_A))
	{
		mspPlayer->Shoot();
	}

	// 'B' button makes the player jump
	if (pButtons->GetButton(Buttons::BUTTON_B))
	{
		mspPlayer->Jump();
	}

	if (pButtons->GetButton(Buttons::BUTTON_1) && (time - lastButton1PressTime) > 0.5)
	{
		ToggleCollidersVisibility();
		lastButton1PressTime = time;
	}

	// If there's a nunchuk, start reading its buttons instead
	if (pInputDevice->HasExtension(Nunchuk::TYPE))
	{
		pButtons = DynamicCast<const Buttons>(pInputDevice->GetExtension(Nunchuk::TYPE)->
			GetCapability(Buttons::TYPE));
		WIRE_ASSERT(pButtons);
	}

	// 'Z' button makes the player run
	if (pButtons->GetButton(Buttons::BUTTON_Z))
	{
		mspPlayer->SetMoveSpeed(10.0F);
	}
	else
	{
		mspPlayer->SetMoveSpeed(5.0F);
	}
}

//----------------------------------------------------------------------------
void Game::ToggleCollidersVisibility()
{
	mShowColliders = !mShowColliders;

	for (UInt i = 0; i < mColliderSpatials.GetQuantity(); i++)
	{
		Node* pColliderNode = DynamicCast<Node>(mColliderSpatials[i]);
		WIRE_ASSERT(pColliderNode);

		if (mShowColliders)
		{
			Collider* pCollider = DynamicCast<Collider>(pColliderNode->GetController(0));
			WIRE_ASSERT(pCollider);

			// Create proper geometry according to the collider shape
			Geometry*  pColliderGeometry = CollisionShapeToGeometryConverter::Convert(pCollider->GetShape(), Color32::GREEN);
			if (pColliderGeometry)
			{
				pColliderNode->AttachChild(pColliderGeometry);

				// Update the render state (necessary because of wireframe)
				pColliderNode->UpdateRS();
			}
		}
		else
		{
			pColliderNode->DetachChildAt(pColliderNode->GetQuantity() - 1);
		}
	}
}

//----------------------------------------------------------------------------
void Game::OnRunning(Double time, Double deltaTime)
{
	mGUICameras[0]->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);

	mspGUI->UpdateGS(time);
	mGUICuller.ComputeVisibleSet(mspGUI);
	mspScene->UpdateGS(time);
	mSceneCuller.ComputeVisibleSet(mspScene);

	// Update physics simulation
	UpdatePhysics(deltaTime);

	GetRenderer()->ResetStatistics();
	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mSceneCameras[0]);
	GetRenderer()->DrawScene(mSceneCuller.GetVisibleSets());
	GetRenderer()->SetCamera(mGUICameras[0]);
	GetRenderer()->DrawScene(mGUICuller.GetVisibleSets());

	if (mShowFps)
	{
		DrawFPS(deltaTime);
	}

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void Game::DrawFPS(Double deltaTime)
{
	// Set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());

	mspTextCamera->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);

	GetRenderer()->SetCamera(mspTextCamera);

	// Set to screen screenWidth (might change any time in window mode)
	mspText->SetLineWidth(screenWidth);
	mspText->Clear(Color32::WHITE);

	// Text uses OpenGL convention of (0,0) being left bottom of window
	mspText->SetPen(0, screenHeight - mspText->GetFontHeight() - 32.0F);

	const UInt textArraySize = 1000;
	Char text[textArraySize];
	UInt fps = static_cast<UInt>(1 / deltaTime);
	String msg1 = "\nFPS: %d\nDraw Calls: %d, Triangles: %d\nBatched Static: "
				  "%d, Batched Dynamic: %d\nVBOs: %d, VBOSize: %.2f KB\nIBOs: %d, "
				  "IBOSize: %.2f KB\nTextures: %d, TextureSize: %.2f MB";

	Float kb = 1024.0F;

	const Renderer::Statistics* pStatistics = Renderer::GetStatistics();
	System::Sprintf(text, 
					textArraySize, 
					static_cast<const Char*>(msg1), 
					fps,
					pStatistics->DrawCalls, 
					pStatistics->Triangles, 
					pStatistics->BatchedStatic,
					pStatistics->BatchedDynamic, 
					pStatistics->VBOCount, 
					pStatistics->VBOTotalSize / kb,
					pStatistics->IBOCount, 
					pStatistics->IBOTotalSize / kb, 
					pStatistics->TextureCount,
					pStatistics->TextureTotalSize / (kb * kb));

	mspText->SetColor(Color32::WHITE);
	mspText->Append(text);
	mspText->Update(GetRenderer());

	GetRenderer()->DisableLighting();
	GetRenderer()->Draw(mspText);
}

//----------------------------------------------------------------------------
void Game::OnLoading(Double time, Double deltaTime)
{
	Spatial* pLoading = mspLogo->GetChildByName("Loading");
	Bool isFadedIn = false;

	if (pLoading)
	{
		StateMaterial* pMaterialState = DynamicCast<StateMaterial>(pLoading->GetState(State::MATERIAL));
		if (pMaterialState)
		{
			pMaterialState->Ambient.A() += static_cast<Float>(deltaTime) * 0.5F;

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

	mspScene = LoadAndInitializeScene();
	if (!mspScene)
	{
		WIRE_ASSERT(false /* Could not load scene.xml */);
	}

	mspGUI = LoadAndInitializeGUI();
	if (!mspGUI)
	{
		WIRE_ASSERT(false /* Could not load GUI.xml */);
	}

	mAppState = AS_RUNNING;
	pLoading->Culling = Spatial::CULL_ALWAYS;

	mspLogo = NULL; // discard Logo scene graph to free memory
}

//----------------------------------------------------------------------------
Node* Game::LoadAndInitializeLoading()
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

	pRoot->Local.SetTranslate(Vector3F((GetWidthF()-512.0F) * 0.5F,
		(GetHeightF() - 256.0F)  * 0.5F, 0));

	GetRenderer()->BindAll(pRoot);

	return pRoot;
}

//----------------------------------------------------------------------------
Node* Game::LoadAndInitializeGUI()
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
	WIRE_ASSERT(mspCrosshair /* No Crosshair in GUI.xml */);

	mspGreenHealthBar = DynamicCast<Geometry>(pRoot->GetChildByName("GreenHealthBar"));
	WIRE_ASSERT(mspGreenHealthBar /* No GreenHealthBar in GUI.xml */);
	mspGreenHealthBar->Local.SetTranslate(Vector3F(10, GetHeightF() - 26.0F, 0));

	mspRedHealthBar = DynamicCast<Geometry>(pRoot->GetChildByName("RedHealthBar"));
	WIRE_ASSERT(mspRedHealthBar /* No RedHealthBar in GUI.xml */);
	mspRedHealthBar->Local.SetTranslate(Vector3F(276, GetHeightF() - 26.0F, 0));

	// Create health monitor
	mspHealthMonitor = WIRE_NEW HealthMonitor(mspGreenHealthBar, mspRedHealthBar, mspPlayer, mspProbeRobot);
	pRoot->AttachController(mspHealthMonitor);

	GetRenderer()->BindAll(pRoot);

	return pRoot;
}

//----------------------------------------------------------------------------
Node* Game::LoadAndInitializeScene()
{
	Importer importer("Data/Scene/");
	Node* pScene = importer.LoadSceneFromXml("Scene.xml", &mSceneCameras, mpPhysicsWorld);

	if (!pScene)
	{
		return NULL;
	}

	WIRE_ASSERT(mSceneCameras.GetQuantity() > 0 /* No Camera in scene.xml */);

	Float fov, aspect, near, far;
	mSceneCameras[0]->GetFrustum(fov, aspect, near, far);
	mSceneCameras[0]->SetFrustum(fov, GetWidthF() / GetHeightF(), near, far);
	mSceneCuller.SetCamera(mSceneCameras[0]);

	Spatial* pStartingPoint = pScene->GetChildByName("Starting Point");

	if (pStartingPoint)
	{
		mStartingPoint = pStartingPoint->World.GetTranslate();
	}
	else
	{
		mStartingPoint = Vector3F::ZERO;
	}

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

	pScene->GetAllChildrenByNameStartingWith("Collider for", mColliderSpatials);
	Spatial* pProbeRobotSpatial = DynamicCast<Spatial>(pScene->GetChildByName("Probe Robot"));
	Spatial* pPlayerSpatial = DynamicCast<Spatial>(pScene->GetChildByName("Player"));

	// Create and configure probe robot controller
	mspProbeRobot = WIRE_NEW ProbeRobot(pPlayerSpatial);
	mspProbeRobot->Register(mpPhysicsWorld);
	mspProbeRobot->SetSpeed(7.5f);
	mspProbeRobot->SetMaximumPlayerDistance(10.0F);
	pProbeRobotSpatial->AttachController(mspProbeRobot);

	// Create and configure player controller
	mspPlayer = WIRE_NEW Player(mSceneCameras[0]);
	mspPlayer->SetLookUpDeadZone(Vector2F(50, 50));
	mspPlayer->SetHeadHeight(0.5F);
	mspPlayer->SetCharacterHeight(1.5F);
	mspPlayer->SetCharacterWidth(0.75F);
	mspPlayer->SetStepHeight(0.5F);
	mspPlayer->SetMaximumVerticalAngle(45);
	mspPlayer->SetMoveSpeed(5.0F);
	mspPlayer->Register(mpPhysicsWorld);
	pPlayerSpatial->AttachController(mspPlayer);

	GetRenderer()->BindAll(pScene);

	return pScene;
}

//----------------------------------------------------------------------------
void Game::MoveCrosshairTo(const Vector2F& rScreenPosition)
{
	mspCrosshair->Local.SetTranslate(Vector3F(rScreenPosition.X() - 16, rScreenPosition.Y() - 16, 0));
}

//----------------------------------------------------------------------------
void Game::InitializePhysics()
{
	mpCollisionConfiguration = WIRE_NEW btDefaultCollisionConfiguration();

	mpDispatcher = WIRE_NEW btCollisionDispatcher(mpCollisionConfiguration);

	btVector3 worldAabbMin(-1000, -1000, -1000);
	btVector3 worldAabbMax(1000, 1000, 1000);
	mpOverlappingPairCache = WIRE_NEW btAxisSweep3(worldAabbMin, worldAabbMax);

	mpConstraintSolver = WIRE_NEW btSequentialImpulseConstraintSolver();
	mpPhysicsWorld = WIRE_NEW btDiscreteDynamicsWorld(mpDispatcher,	mpOverlappingPairCache, mpConstraintSolver, mpCollisionConfiguration);

	mpPhysicsWorld->setGravity(btVector3(0, -9.8F, 0));
	mpPhysicsWorld->getPairCache()->setInternalGhostPairCallback(WIRE_NEW btGhostPairCallback());
}

//----------------------------------------------------------------------------
void Game::UpdatePhysics(Double deltaTime)
{
	mpPhysicsWorld->stepSimulation(btScalar(deltaTime), 10);
}

//----------------------------------------------------------------------------
void Game::TerminatePhysics()
{
	for (Int i = mpPhysicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* pCollisionObject = mpPhysicsWorld->getCollisionObjectArray()[i];
		btRigidBody* pRigidBody = btRigidBody::upcast(pCollisionObject);

		if (pRigidBody && pRigidBody->getMotionState())
		{
			WIRE_DELETE pRigidBody->getMotionState();
		}

		mpPhysicsWorld->removeCollisionObject(pCollisionObject);
		WIRE_DELETE pCollisionObject;
	}

	WIRE_DELETE mpPhysicsWorld;
	WIRE_DELETE mpConstraintSolver;
	WIRE_DELETE mpOverlappingPairCache;
	WIRE_DELETE mpDispatcher;
	WIRE_DELETE mpCollisionConfiguration;
}
