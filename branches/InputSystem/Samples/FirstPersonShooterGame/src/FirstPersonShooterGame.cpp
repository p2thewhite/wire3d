#include "FirstPersonShooterGame.h"

#include "MaterialFader.h"
#include "WireInputSystem.h"
#include "WireAnalogPad.h"
#include "WireDigitalPad.h"
#include "WireIR.h"
#include "WireButtons.h"

#include "CollisionShapeToGeometryConverter.h"

using namespace Wire;

WIRE_APPLICATION(FirstPersonShooterGame);

//----------------------------------------------------------------------------
FirstPersonShooterGame::FirstPersonShooterGame() :
	mpCharacterController(NULL),
	mShowColliders(false)
{
}

//----------------------------------------------------------------------------
Bool FirstPersonShooterGame::OnInitialize()
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

	// frames per second and render statistics debug text
	mspTextCamera = WIRE_NEW Camera(/* isPerspective */ false);
	mspText = Importer::CreateText("Data/Logo/cour.ttf", 20, 20);
	WIRE_ASSERT(mspText);

	GetRenderer()->BindAll(mspText);
	GetRenderer()->CreateBatchingBuffers(60 * 1024);
	GetRenderer()->SetDynamicBatchingThreshold(300);
	GetRenderer()->SetStaticBatchingThreshold(10000);

	return true;
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::OnTerminate()
{
	TerminatePhysics();
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::OnIdle()
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
void FirstPersonShooterGame::OnInput()
{
	static Double lastButton1PressTime = 0;
	static Float oldX = 0;
	static Float oldY = 0;
	Double time = System::GetTime();

	if (mpCharacterController == NULL)
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
			mpCharacterController->MoveForward();
		}
		else if (pAnalogPad->GetDown() > 0)
		{
			mpCharacterController->MoveBackward();
		}

		if (pAnalogPad->GetRight() > 0)
		{
			mpCharacterController->StrafeRight();
		}
		else if (pAnalogPad->GetLeft() > 0)
		{
			mpCharacterController->StrafeLeft();
		}
	}
	else 
	{
		const DigitalPad* pDigitalPad = DynamicCast<const DigitalPad>(pInputDevice->
			GetCapability(DigitalPad::TYPE, false));
		WIRE_ASSERT(pDigitalPad);

		if (pDigitalPad->GetUp())
		{
			mpCharacterController->MoveForward();
		}
		else if (pDigitalPad->GetDown())
		{
			mpCharacterController->MoveBackward();
		}

		if (pDigitalPad->GetLeft())
		{
			mpCharacterController->StrafeLeft();
		}
		else if (pDigitalPad->GetRight())
		{
			mpCharacterController->StrafeRight();
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

	mpCharacterController->LookAt(Vector2F(x, y));

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

	// 'B' button makes the player jump
	if (pButtons->GetButton(Buttons::BUTTON_B))
	{
		mpCharacterController->Jump();
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
		mpCharacterController->SetMoveSpeed(5.0f);
	}
	else
	{
		mpCharacterController->SetMoveSpeed(2.5f);
	}
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::ToggleCollidersVisibility()
{
	mShowColliders = !mShowColliders;

	for (UInt i = 0; i < mColliderSpatials.GetQuantity(); i++)
	{
		Node* pColliderNode = DynamicCast<Node>(mColliderSpatials[i]);
		WIRE_ASSERT(pColliderNode);

		if (mShowColliders)
		{
			Collider* pColliderController = DynamicCast<Collider>(pColliderNode->GetController(0));
			WIRE_ASSERT(pColliderController);

			// Create proper geometry according to the collider shape
			Geometry*  pColliderGeometry = CollisionShapeToGeometryConverter::Convert(
				pColliderController->GetShape(), Color32::GREEN);
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
void FirstPersonShooterGame::OnRunning(Double time, Double deltaTime)
{
	mGUICameras[0]->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);

	mspLogo->UpdateGS(time);
	mLogoCuller.ComputeVisibleSet(mspLogo);
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
	GetRenderer()->SetCamera(mLogoCameras[0]);
	GetRenderer()->DrawScene(mLogoCuller.GetVisibleSets());

	if (mShowFps)
	{
		DrawFPS(deltaTime);
	}

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::DrawFPS(Double deltaTime)
{
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
void FirstPersonShooterGame::OnLoading(Double time, Double deltaTime)
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
}

//----------------------------------------------------------------------------
Node* FirstPersonShooterGame::LoadAndInitializeLoading()
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
		pLogo->AttachController(WIRE_NEW MaterialFader(2.5f));
	}

	pRoot->Local.SetTranslate(Vector3F((GetWidthF()-512.0F) * 0.5F,
		(GetHeightF() - 256.0F)  * 0.5F, 0));

	GetRenderer()->BindAll(pRoot);

	return pRoot;
}

//----------------------------------------------------------------------------
Node* FirstPersonShooterGame::LoadAndInitializeGUI()
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

	Geometry* pMainInputDeviceIcon = DynamicCast<Geometry>(pRoot->
		GetChildByName("MainInputDeviceIcon"));
	WIRE_ASSERT(pMainInputDeviceIcon /* No MainInputDeviceIcon in GUI.xml */);

	pMainInputDeviceIcon->Local.SetTranslate(Vector3F(0, GetHeightF()-64.0F, 0));

	// loading the main input device icon dynamically according to the platform
	if (System::GetPlatform() == System::PF_WII)
	{
		pMainInputDeviceIcon->GetMaterial()->SetTexture(0, LoadTexture(
			importer, "Data/GUI/wiiMoteIcon.png"));
	}
	else
	{
		pMainInputDeviceIcon->GetMaterial()->SetTexture(0, LoadTexture(
			importer, "Data/GUI/keyboardIcon.png"));
	}

	Geometry* pInputDeviceExtensionIcon = DynamicCast<Geometry>(pRoot->
		GetChildByName("InputDeviceExtensionIcon"));
	WIRE_ASSERT(pMainInputDeviceIcon != NULL);
	pInputDeviceExtensionIcon->Local.SetTranslate(Vector3F(64, GetHeightF()-64.0F, 0));

	// loading the input device extension icon dynamically according to the platform
	if (System::GetPlatform() == System::PF_WII)
	{
		pInputDeviceExtensionIcon->GetMaterial()->SetTexture(0, LoadTexture(
			importer, "Data/GUI/nunchukIcon.png"));
	}
	else
	{
		pInputDeviceExtensionIcon->GetMaterial()->SetTexture(0, LoadTexture(
			importer, "Data/GUI/mouseIcon.png"));
	}

	GetRenderer()->BindAll(pRoot);
	return pRoot;
}

//----------------------------------------------------------------------------
Node* FirstPersonShooterGame::LoadAndInitializeScene()
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

	Spatial* pStartingPosition = pScene->GetChildByName("StartingPosition");

	if (pStartingPosition)
	{
		mStartingPosition = pStartingPosition->World.GetTranslate();
	}
	else
	{
		mStartingPosition = Vector3F::ZERO;
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

	pScene->GetAllChildrenByName("Collider", mColliderSpatials);

	// Create and configure the character controller
	mpCharacterController = WIRE_NEW FirstPersonController(mStartingPosition, mSceneCameras[0]);

	mpCharacterController->SetLookUpDeadZone(Vector2F(50, 50));
	mpCharacterController->SetHeadHeight(0.5f);
	mpCharacterController->SetCharacterHeight(1.5f);
	mpCharacterController->SetCharacterWidth(0.75f);
	mpCharacterController->SetStepHeight(0.35f);
	mpCharacterController->SetMaximumVerticalAngle(45);
	mpCharacterController->Register(mpPhysicsWorld);

	pScene->AttachController(mpCharacterController);

	GetRenderer()->BindAll(pScene);

	return pScene;
}

//----------------------------------------------------------------------------
Texture2D* FirstPersonShooterGame::LoadTexture(Importer& rImporter, const Char* pFileName)
{
	Texture2D* pTexture = WIRE_NEW Texture2D(rImporter.LoadPNG(pFileName, false));

	pTexture->SetFilterType(Texture2D::FT_LINEAR_NEAREST);
	pTexture->SetWrapType(0, Texture2D::WT_CLAMP);
	pTexture->SetWrapType(1, Texture2D::WT_CLAMP);
	pTexture->SetAnisotropyValue(1);

	return pTexture;
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::MoveCrosshairTo(const Vector2F& rScreenPosition)
{
	mspCrosshair->Local.SetTranslate(Vector3F(rScreenPosition.X() - 16, rScreenPosition.Y() - 16, 0));
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::InitializePhysics()
{
	mpCollisionConfiguration = WIRE_NEW btDefaultCollisionConfiguration();

	mpDispatcher = WIRE_NEW btCollisionDispatcher(mpCollisionConfiguration);

	btVector3 worldAabbMin(-1000, -1000, -1000);
	btVector3 worldAabbMax(1000, 1000, 1000);
	mpOverlappingPairCache = WIRE_NEW btAxisSweep3(worldAabbMin, worldAabbMax);

	mpConstraintSolver = WIRE_NEW btSequentialImpulseConstraintSolver();
	mpPhysicsWorld = WIRE_NEW btDiscreteDynamicsWorld(mpDispatcher,
		mpOverlappingPairCache, mpConstraintSolver, mpCollisionConfiguration);

	mpPhysicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::UpdatePhysics(Double deltaTime)
{
	mpPhysicsWorld->stepSimulation(btScalar(deltaTime), 10);
}

//----------------------------------------------------------------------------
void FirstPersonShooterGame::TerminatePhysics()
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
