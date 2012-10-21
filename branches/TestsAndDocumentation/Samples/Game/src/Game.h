#pragma once
#ifndef GAME_H
#define GAME_H

#include "WireApplication.h"

#include "ProbeRobot.h"
#include "Player.h"
#include "HealthMonitor.h"
#include "Collider.h"
#include "btBulletDynamicsCommon.h"

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
	Wire::Node* LoadAndInitializeLoading();
	Wire::Node* LoadAndInitializeScene();
	Wire::Node* LoadAndInitializeGUI();

	void MoveCrosshairTo(const Wire::Vector2F& rScreenPoint);
	void InitializePhysics();
	void RegisterStaticCollider(const Collider* pCollider);
	void UpdatePhysics(Double deltaTime);
	void TerminatePhysics();
	void ToggleCollidersVisibility();

	Double mLastApplicationTime;
	UInt mAppState;
	Bool mShowFps;
	Wire::Vector3F mStartingPoint;
	Wire::Pointer<ProbeRobot> mspProbeRobot;
	Wire::Pointer<Player> mspPlayer;
	Wire::Pointer<HealthMonitor> mspHealthMonitor;
	Wire::NodePtr mspLogo;
	Wire::NodePtr mspScene;
	Wire::NodePtr mspGUI;
	Wire::TArray<Wire::CameraPtr> mLogoCameras;
	Wire::TArray<Wire::CameraPtr> mSceneCameras;
	Wire::TArray<Wire::CameraPtr> mGUICameras;
	Wire::Culler mLogoCuller;
	Wire::CullerSorting mSceneCuller;
	Wire::Culler mGUICuller;
	Wire::SpatialPtr mspCrosshair;
	Wire::GeometryPtr mspGreenHealthBar;
	Wire::GeometryPtr mspRedHealthBar;
	Wire::TextPtr mspText;
	Bool mShowColliders;
	Wire::TArray<Wire::Spatial*> mColliderSpatials;
	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btBroadphaseInterface* mpOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mpConstraintSolver;
	btDiscreteDynamicsWorld* mpPhysicsWorld;
};

WIRE_REGISTER_INITIALIZE(Game);

#endif
