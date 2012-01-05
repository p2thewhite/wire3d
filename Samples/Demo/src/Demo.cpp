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

	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	mLogoCameras[0]->SetFrustum(0, width, 0, height, 0, 1);

	Vector3F centered((width-512)*0.5F, (height-256)*0.5F, 0);
	mspLogo->Local.SetTranslate(centered);
	mspLogo->UpdateGS(time);
	mLogoCuller.SetCamera(mLogoCameras[0]);
	mLogoCuller.ComputeVisibleSet(mspLogo);

	mspScene1->UpdateGS(time);
	mScene1Culler.ComputeVisibleSet(mspScene1);

	GetRenderer()->ResetStatistics();

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mScene1Cameras[0]);
	GetRenderer()->DrawScene(mScene1Culler.GetVisibleSets());

	GetRenderer()->SetCamera(mLogoCameras[0]);
	GetRenderer()->DrawScene(mLogoCuller.GetVisibleSets());

	DrawFPS(elapsedTime, true);

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

	Node* pRoot = DynamicCast<Node>(pLogo->GetChildByName("Root"));
	if (pRoot)
	{
		pRoot->AttachController(WIRE_NEW LogoFader);
	}

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

//----------------------------------------------------------------------------
void Demo::DrawFPS(Double elapsed, Bool usesSorting)
{
	// set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());

	const Renderer::Statistics* pStats = Renderer::GetStatistics();

	UInt fps = static_cast<UInt>(1/elapsed);
	const UInt TextArraySize = 1000;
	Char text[TextArraySize];
	String msg1 = "\2\nFPS: %d\nDraw Calls: %d, Triangles: %d\nVBOs: %d, "
		"VBOSize: %d\nIBOs: %d, IBOSize: %d\nTextures: %d, TextureSize: "
		"%5.2fMB\n%5.2f";

	System::Sprintf(text, TextArraySize, static_cast<const Char*>(msg1), fps,
		pStats->DrawCalls, pStats->Triangles, pStats->VBOCount, pStats->
		VBOTotalSize, pStats->IBOCount, pStats->IBOTotalSize, pStats->
		TextureCount, pStats->TextureTotalSize/(1024.0f*1024.0f), (float)mLastTime);

	String msg0 = "\n\n\n\n\n\nSorting: ";
	String str;

	if (usesSorting)
	{
		str = msg0 + String("\x01\x20\xff\x20\xffON") + String(text);
	}
	else
	{
		str = msg0 + String("\x01\xff\x20\x20\xffOFF") + String(text);
	}

	GeometryPtr spText = StandardMesh::CreateText(str, screenWidth,
		screenHeight, ColorRGBA::WHITE);
	StateAlpha* pTextAlpha = WIRE_NEW StateAlpha();
	pTextAlpha->BlendEnabled = true;
	spText->AttachState(pTextAlpha);
	spText->UpdateRS();

	GetRenderer()->DisableLighting();
	GetRenderer()->Draw(spText);
}
