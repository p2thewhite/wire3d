#ifndef PLAYER_H
#define PLAYER_H

#include "WireController.h"
#include "WireCamera.h"
#include "WireNode.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

class Player : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	Player(Wire::Camera* pCamera);

	virtual Bool Update(Double appTime);
	void Register(btDynamicsWorld* pDynamicsWorld);
	void SetMoveSpeed(Float moveSpeed);
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void LookAt(const Wire::Vector2F& rLookAt);
	void Jump();
	void Shoot();

private:
	Wire::Vector3F GetPosition();
	void UpdateGunRotation();
	void DoShooting();
	void CreateRay(Float size);

	Float mHeadHeight;
	Float mMaximumShootingDistance;
	Float mMaximumVerticalAngle;
	Wire::Vector2F mLookUpDeadZone;
	Float mMoveSpeed;
	Float mRotateSpeed;
	Float mCharacterWidth;
	Float mCharacterHeight;
	Float mStepHeight;
	Wire::Vector3F mEyeDirection;
	Wire::Vector3F mUp;
	Wire::Vector3F mForward;
	Wire::Vector3F mRight;
	Float mPitch;
	Float mYaw;
	Float mPitchIncrement;
	Float mYawIncrement;
	Wire::Matrix3F mRotationX;
	Wire::Matrix3F mRotationY;
	Wire::Matrix3F mGunStartingRotation;
	Wire::Vector3F mMove;
	Wire::Vector2F mLookAt;
	Wire::Node* mpNode;
	Wire::Spatial* mpGun;
	Wire::CameraPtr mspCamera;
	Bool mJump;
	Bool mShoot;
	btDynamicsWorld* mpPhysicsWorld;
	btPairCachingGhostObject* mpGhostObject;
	btKinematicCharacterController* mpPhysicsEntity;
};

#endif
