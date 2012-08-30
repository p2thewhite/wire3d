#ifndef PLAYER_H_
#define PLAYER_H_

#include "WireController.h"
#include "WireCamera.h"
#include "WireNode.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

using namespace Wire;

class Player : public Controller
{
	WIRE_DECLARE_RTTI;

public:
	Player(Camera* pCamera);

	virtual Bool Update(Double appTime);
	void Register(btDynamicsWorld* pDynamicsWorld);
	void SetTotalHealth(Float health);
	Float GetHealth();
	Float GetTotalHealth();
	void SetHeadHeight(Float headHeight);
	void SetCharacterWidth(Float characterWidth);
	void SetCharacterHeight(Float characterHeight);
	void SetStepHeight(Float stepHeight);
	void SetMoveSpeed(Float moveSpeed);
	void SetRotateSpeed(Float rotateSpeed);
	void SetLookUpDeadZone(const Vector2F& rLookUpDeadZone);
	void SetMaximumVerticalAngle(Float maximumVerticalAngle);
	void SetMaximumShootingDistance(Float maximumShootingDistance);
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void LookAt(const Vector2F& rLookAt);
	void Jump();
	void Shoot();
	void TakeDamage(Float damage);

private:
	Float mTotalHealth;
	Float mHealth;
	Float mHeadHeight;
	Float mMaximumShootingDistance;
	Float mMaximumVerticalAngle;
	Vector2F mLookUpDeadZone;
	Float mMoveSpeed;
	Float mRotateSpeed;
	Float mCharacterWidth;
	Float mCharacterHeight;
	Float mStepHeight;
	Vector3F mEyeDirection;
	Vector3F mUp;
	Vector3F mForward;
	Vector3F mRight;
	Float mPitch;
	Float mYaw;
	Float mPitchIncrement;
	Float mYawIncrement;
	Matrix3F mRotationX;
	Matrix3F mRotationY;
	Matrix3F mGunStartingRotation;
	Vector2F mLookAt;
	Vector3F mMove;
	Bool mJump;
	Bool mShoot;
	Node* mpNode;
	Spatial* mpGun;
	CameraPtr mspCamera;
	btDynamicsWorld* mpPhysicsWorld;
	btPairCachingGhostObject* mpGhostObject;
	btKinematicCharacterController* mpPhysicsEntity;

	void InitializeIfNecessary();
	Float GetDeltaTime(Double appTime);
	Vector3F GetPosition();
	void UpdatePlayerNode();
	void UpdateGunRotation();
	void DoShooting();
	void UpdateCamera();
	void MovePhysicsEntity();
	void CreateRay(Float size);
	
};

#endif