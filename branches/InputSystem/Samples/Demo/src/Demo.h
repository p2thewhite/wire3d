#pragma once
#ifndef DEMO_H
#define DEMO_H

#include "WireApplication.h"
#include "Controllers/FirstPersonController.h"

using namespace Wire;

class Demo : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Demo();

	virtual Bool OnInitialize();
	virtual void OnIdle();
	virtual void OnInput();

private:
	enum AppState
	{
		AS_LOADING,
		AS_RUNNING
	};

	void StateLoading(Double time);
	void StateRunning(Double time);

	Node* LoadAndInitLogo();
	NodePtr mspLogo;
	TArray<CameraPtr> mLogoCameras;
	Culler mLogoCuller;

	Node* LoadAndInitScene();
	NodePtr mspScene;
	TArray<CameraPtr> mSceneCameras;
	CullerSorting mSceneCuller;

	Node* LoadAndInitGUI();
	NodePtr mspGUI;
	SpatialPtr mspCrosshair;
	TArray<CameraPtr> mGUICameras;
	Culler mGUICuller;

	Double mLastTime;
	UInt mAppState;
	FirstPersonController* mpFirstPersonController;

	// frames per second debug text
	void DrawFPS(Double time);
	CameraPtr mspTextCamera;
	TextPtr mspText;
	Bool mShowFps;

	void UpdateCameraFrustumAccordingToScreenDimensions(Camera* pCamera);
	Texture2D* CreateTexture(Image2D* pImage);
	void MoveCrosshairTo(const Vector2F& rScreenPoint);
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
