#pragma once
#ifndef FIRSTPERSONSHOOTERGAME_H_
#define FIRSTPERSONSHOOTERGAME_H_

#include "WireApplication.h"
#include "Importer.h"
#include "FirstPersonController.h"

#include "Collider.h"
#include "btBulletDynamicsCommon.h"

using namespace Wire;

class FirstPersonShooterGame : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	FirstPersonShooterGame();

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

	Double mLastTime;
	UInt mAppState;
	Bool mShowFps;
	Vector3F mStartingPosition;
	FirstPersonController* mpCharacterController;
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
	CameraPtr mspTextCamera;
	TextPtr mspText;
	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btBroadphaseInterface* mpOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mpConstraintSolver;
	btDiscreteDynamicsWorld* mpPhysicsWorld;

	void StateLoading(Double time);
	void StateRunning(Double time);
	Node* LoadAndInitLogo();
	Node* LoadAndInitScene();
	Node* LoadAndInitGUI();
	void UpdateCameraFrustumAccordingToScreenDimensions(Camera* pCamera);
	Texture2D* LoadTexture(Importer& rImporter, Char* pFileName);
	void MoveCrosshairTo(const Vector2F& rScreenPoint);
	void InitializePhysics();
	void RegisterStaticCollider(const Collider* pCollider);
	void UpdatePhysics(Float deltaTime);
	void TerminatePhysics();
	void DrawFPS(Double time);

};

WIRE_REGISTER_INITIALIZE(FirstPersonShooterGame);

#endif
