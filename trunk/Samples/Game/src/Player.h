#ifndef PLAYER_H
#define PLAYER_H

#include "WireController.h"
#include "WireCamera.h"
#include "WireNode.h"

#include "PhysicsWorld.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

class Player : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	Player(Wire::Camera* pCamera);

	virtual Bool Update(Double appTime);
	void Register(PhysicsWorld* pPhysicsWorld);
	void LookAt(const Wire::Vector2F& rLookAt);

private:
	void ProcessInput();

	void SetMoveSpeed(Float moveSpeed);
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void Jump();
	void Shoot();

	Wire::Vector3F GetPosition();
	void UpdateGunRotation();
	void DoShooting(const Wire::Vector3F& rDirection);
	void CreateRay(Float size);

	const Float mHeadHeight;
	const Float mMaximumShootingDistance;
	const Float mMaximumVerticalAngle;
	const Wire::Vector2F mLookUpDeadZone;
	const Float mCharacterWidth;
	const Float mCharacterHeight;
	const Float mStepHeight;
	const Float mRotateSpeed;

	Float mMoveSpeed;
	Wire::Vector3F mForward;
	Wire::Vector3F mRight;
	Float mPitch;
	Float mYaw;
	Float mPitchIncrement;
	Float mYawIncrement;
	Float mRoll;
	Wire::Vector3F mMove;
	Wire::Vector2F mLookAt;
	Wire::Node* mpNode;
	Wire::Spatial* mpGun;
	Wire::CameraPtr mspCamera;

	Bool mJump;
	Bool mShoot;
	Bool mWasButtonAPressed;

	PhysicsWorldPtr mspPhysicsWorld;
	btPairCachingGhostObject* mpGhostObject;
	btKinematicCharacterController* mpPhysicsEntity;
};

#endif
	