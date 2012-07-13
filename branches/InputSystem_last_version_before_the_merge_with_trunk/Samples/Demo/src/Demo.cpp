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

	// frames per second debug text
	mspTextCamera = WIRE_NEW Camera(/* isPerspective */ false);
	mspTextAlpha = WIRE_NEW StateAlpha();
	mspTextAlpha->BlendEnabled = true;
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
void Demo::OnButton(UInt /*button*/, UInt state)
{
	// Buttons are:     Wii          | PC
	//   BUTTON_A =     Button 'A'   | left mouse button
	//   BUTTON_B =     Button 'B'   | right mouse button
	//   BUTTON_LEFT =  Button left  | cursor key left
	//   BUTTON_RIGHT = Button right | cursor key right
	//   BUTTON_UP =    Button up    | cursor key up
	//   BUTTON_DOWN =  Button down  | cursor key down
	//   BUTTON_1 =     Button '1'   | key '1'
	//   BUTTON_2 =     Button '2'   | key '2'
	//
	// States are:
	//   BUTTON_PRESS   = button/key pressed
	//   BUTTON_RELEASE = button/key released

	// any of the above buttons pressed displays FPS at the top of the screen
	if (state == Application::BUTTON_PRESS)
	{
		mShowFps = true;
	}
	else
	{
		mShowFps = false;
	}
}

//----------------------------------------------------------------------------
void Demo::StateRunning(Double time)
{
	mspLogo->UpdateGS(time);
	mLogoCuller.ComputeVisibleSet(mspLogo);

	mspScene->UpdateGS(time);
	mSceneCuller.ComputeVisibleSet(mspScene);

	GetRenderer()->ResetStatistics();

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
	static Double lastTime = 0.0f;
	Double elapsed = time - lastTime;
	lastTime = time;

	// set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
	mspTextCamera->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);
	GetRenderer()->SetCamera(mspTextCamera);

	const Renderer::Statistics* pStats = Renderer::GetStatistics();

	UInt fps = static_cast<UInt>(1/elapsed);
	const UInt TextArraySize = 1000;
	Char text[TextArraySize];
	String msg1 = "\n\n\n\n\nFPS: %d\nDraw Calls: %d, Triangles: %d\nVBOs: %d, "
		"VBOSize: %.2f KB\nIBOs: %d, IBOSize: %.2f KB\nTextures: %d, "
		"TextureSize: %.2f MB";

	const Float kb = 1024.0f;
	const Float mb = kb*kb;
	System::Sprintf(text, TextArraySize, static_cast<const Char*>(msg1), fps,
		pStats->DrawCalls, pStats->Triangles, pStats->VBOCount, pStats->
		VBOTotalSize/kb, pStats->IBOCount, pStats->IBOTotalSize/kb, pStats->
		TextureCount, pStats->TextureTotalSize/mb);

	String str(text);

// #ifdef WIRE_DEBUG
// 	Float allocatedMemory = static_cast<Float>(Memory::AllocatedMemory)/mb;
// 	System::Sprintf(text, TextArraySize, "\nAllocated Memory: %.2f MB, "
// 		"Number of Allocations: %d", allocatedMemory,
// 		Memory::AllocationCount);
// 	str += String(text);
// #endif

	GeometryPtr spText = StandardMesh::CreateText(str, screenWidth,
		screenHeight, ColorRGBA::WHITE);
	spText->AttachState(mspTextAlpha);
	spText->UpdateRS();

	GetRenderer()->DisableLighting();
	GetRenderer()->Draw(spText);
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
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mSceneCameras[0]->GetFrustum(fov, aspect, near, far);
	aspect = width / height;
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