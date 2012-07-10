#ifndef FIRSTPERSONCONTROLLER_H_
#define FIRSTPERSONCONTROLLER_H_

#include "WireController.h"
#include "WireCamera.h"
#include "WireNode.h"
#include "WireQuaternion.h"
#include "WireVector2.h"

using namespace Wire;

class FirstPersonController : public Controller
{
public:
	FirstPersonController(const Vector3F& rPosition, Camera* pCamera);

	virtual Bool Update(Double appTime);
	void SetMoveSpeed(Float moveSpeed);
	void SetRotateSpeed(Float rotateSpeed);
	void SetLookUpDeadZone(const Vector2F& rLookUpDeadZone);
	void SetMaxVerticalAngle(Float maxVerticalAngle);
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void LookUp(const Vector2F& rLookUp);

private:
	Double mLastAppTime;
	Float mAngleY;
	Float mAngleX;
	Float mAngleYIncrement;
	Float mAngleXIncrement;
	Vector3F mMove;
	Vector2F mLookUpDeadZone;
	Float mMaxVerticalAngle;
	Float mMoveSpeed;
	Float mRotateSpeed;
	CameraPtr mspCamera;

	Vector3F GetDirection();
	Vector3F GetBackward();
	Vector3F GetLeft();
	
};

#endif