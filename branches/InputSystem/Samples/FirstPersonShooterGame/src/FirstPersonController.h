#ifndef FIRSTPERSONCONTROLLER_H_
#define FIRSTPERSONCONTROLLER_H_

#include "WireController.h"
#include "WireCamera.h"
#include "WireNode.h"
#include "WireQuaternion.h"
#include "WireVector2.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

using namespace Wire;

class FirstPersonController : public Controller
{
public:
	FirstPersonController(const Vector3F& rStartingPosition, Camera* pCamera);

	virtual Bool Update(Double appTime);
	virtual void Register(btDynamicsWorld* pDynamicsWorld);
	void UpdateCamera();
	void MoveCharacterController();
	void SetHeadHeight(Float headHeight);
	void SetCharacterWidth(Float characterWidth);
	void SetCharacterHeight(Float characterHeight);
	void SetStepHeight(Float stepHeight);
	void SetMoveSpeed(Float moveSpeed);
	void SetRotateSpeed(Float rotateSpeed);
	void SetLookUpDeadZone(const Vector2F& rLookUpDeadZone);
	void SetMaximumVerticalAngle(Float maxVerticalAngle);
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void LookAt(const Vector2F& rLookAt);
	void Jump();

private:
	Float mHeadHeight;
	Float mMaxPitch;
	Vector2F mLookUpDeadZone;
	Float mMoveSpeed;
	Float mRotateSpeed;
	Float mCharacterWidth;
	Float mCharacterHeight;
	Float mStepHeight;
	Vector3F mLookAt;
	Vector3F mUp;
	Vector3F mBack;
	Vector3F mLeft;
	Float mPitch;
	Float mYaw;
	Float mPitchIncrement;
	Float mYawIncrement;
	Vector3F mMove;
	Bool mJump;
	CameraPtr mspCamera;
	btDynamicsWorld* mpPhysicsWorld;
	btPairCachingGhostObject* mpGhostObject;
	btKinematicCharacterController* mpPhysicsCharacterController;
	
};

#endif