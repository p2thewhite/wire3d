#ifndef PLAYER_H
#define PLAYER_H

#include "WireController.h"
#include "WireCamera.h"
#include "WireNode.h"
#include "WireStateMaterial.h"
#include "PhysicsWorld.h"

class Player : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	Player(Wire::Camera* pCamera, PhysicsWorld* pPhysicsWorld);

	virtual Bool Update(Double appTime);
	virtual void OnAttach();
	void LookAt(const Wire::Vector2F& rLookAt);

private:
	void ProcessInput();

	void SetMoveSpeed(Float moveSpeed);
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void Jump();
	void ShootGun();

	Wire::Vector3F GetPosition();
	void UpdateGun(Double deltaTime);
	void UpdateShot(Double deltaTime, const Wire::Vector2F& rCursorPosition);

	const Float mHeadHeight;
	const Float mMaximumShootingDistance;
	const Float mMaximumVerticalAngle;
	const Wire::Vector2F mLookUpDeadZone;
	const Float mRotateSpeed;

	Float mMoveSpeed;
	Wire::Vector3F mForward;
	Wire::Vector3F mRight;
	Float mPitch;
	Float mYaw;
	Float mPitchIncrement;
	Float mYawIncrement;
	Wire::TArray<Float> mRolls;
	Wire::Vector3F mMove;
	Wire::Vector2F mLookAt;
	Wire::Node* mpNode;
	Wire::Node* mpGun;
	Wire::CameraPtr mspCamera;
	Wire::StateMaterialPtr mspMuzzleflashMaterialState;
	Wire::LightPtr mspMuzzleflashLight;
	Wire::ColorRGBA mMuzzleflashLightColor;

	Float mShoot;
	Bool mJump;

	PhysicsWorldPtr mspPhysicsWorld;
	CharacterControllerPtr mspCharacter;
};

#endif
	