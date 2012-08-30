#pragma once
#ifndef GAME_H
#define GAME_H

#include "WireApplication.h"

#include "ProbeRobot.h"
#include "Player.h"
#include "HealthMonitor.h"
#include "Collider.h"
#include "btBulletDynamicsCommon.h"

using namespace Wire;

class Game : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Game();

	virtual Bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();
	virtual void OnInput();

private:
	enum AppState
	{
		AS_LOADING,
		AS_RUNNING
	};

	void OnLoading(Double time, Double deltaTime);
	void OnRunning(Double time, Double deltaTime);
	Node* LoadAndInitializeLoading();
	Node* LoadAndInitializeScene();
	Node* LoadAndInitializeGUI();

	void MoveCrosshairTo(const Vector2F& rScreenPoint);
	void InitializePhysics();
	void RegisterStaticCollider(const Collider* pCollider);
	void UpdatePhysics(Double deltaTime);
	void TerminatePhysics();
	void DrawFPS(Double deltaTime);
	void ToggleCollidersVisibility();

	Double mLastApplicationTime;
	UInt mAppState;
	Bool mShowFps;
	Vector3F mStartingPoint;
	Pointer<ProbeRobot> mspProbeRobot;
	Pointer<Player> mspPlayer;
	Pointer<HealthMonitor> mspHealthMonitor;
	NodePtr mspLogo;
	NodePtr mspScene;
	NodePtr mspGUI;
	TArray<CameraPtr> mLogoCameras;
	TArray<CameraPtr> mSceneCameras;
	TArray<CameraPtr> mGUICameras;
	Culler mLogoCuller;
	CullerSorting mSceneCuller;
	Culler mGUICuller;
	SpatialPtr mspCrosshair;
	GeometryPtr mspGreenHealthBar;
	GeometryPtr mspRedHealthBar;
	CameraPtr mspTextCamera;
	TextPtr mspText;
	Bool mShowColliders;
	TArray<Spatial*> mColliderSpatials;
	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btBroadphaseInterface* mpOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mpConstraintSolver;
	btDiscreteDynamicsWorld* mpPhysicsWorld;
};

WIRE_REGISTER_INITIALIZE(Game);

#endif
