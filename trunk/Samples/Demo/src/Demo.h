#pragma once
#ifndef DEMO_H
#define DEMO_H

#include "WireApplication.h"

using namespace Wire;

class Demo : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

	virtual void OnButton(UInt button, UInt state);

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

	Double mLastTime;
	UInt mAppState;

	// frames per second debug text
	void DrawFPS(Double time);
	CameraPtr mspTextCamera;
	StateAlphaPtr mspTextAlpha;
	Bool mShowFps;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
