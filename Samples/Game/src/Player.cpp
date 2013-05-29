#include "Player.h"

#include "ProbeRobot.h"
#include "PhysicsWorld.h"
#include "WireApplication.h"
#include "WireStandardMesh.h"
#include "WireStateWireframe.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Player, Controller);

//----------------------------------------------------------------------------
Player::Player(Camera* pCamera)
	:
	mHeadHeight(0.5F),
	mMaximumShootingDistance(1000.0F),
	mMaximumVerticalAngle(MathF::DEG_TO_RAD * 45.0F),
	mLookUpDeadZone(Vector2F(50, 50)),
	mCharacterWidth(1.7F),
	mCharacterHeight(1.5F),
	mStepHeight(0.5F),
	mMoveSpeed(5.0F),
	mRotateSpeed(MathF::PI / 9),
	mPitch(0),
	mYaw(0),
	mPitchIncrement(0),
	mYawIncrement(0),
	mMove(Vector3F::ZERO),
	mLookAt(Vector2F::ZERO),
	mpNode(NULL),
	mJump(false),
	mShoot(false)
{
	WIRE_ASSERT(pCamera);
	mspCamera = pCamera;
}

//----------------------------------------------------------------------------
Bool Player::Update(Double appTime)
{
	Float deltaTime = static_cast<Float>(appTime - mLastApplicationTime);
	if (mLastApplicationTime == -MathD::MAX_REAL)
	{
		deltaTime = 0.0F;
	}

	mLastApplicationTime = appTime;

	// Apply accumulators
	mMove *= deltaTime;
	mYaw += mYawIncrement * deltaTime;

	if (mYaw > MathF::TWO_PI) 
	{
		mYaw = mYaw - MathF::TWO_PI;
	}
	else if (mYaw < -MathF::TWO_PI)
	{
		mYaw = MathF::TWO_PI - mYaw;
	}

	mPitch += (mPitchIncrement * deltaTime);
	mPitch = MathF::Clamp(-mMaximumVerticalAngle, mPitch, mMaximumVerticalAngle);

	// Calculate rotation matrices
	Matrix3F rotationX(Vector3F::UNIT_X, mPitch);
	Matrix3F rotationY(Vector3F::UNIT_Y, mYaw);
	Matrix3F rotationYX = rotationY * rotationX;

	// Calculate up, gunDirection and lookup
	Vector3F eyeDirection = rotationYX.GetColumn(2);
	Vector3F up = rotationYX.GetColumn(1);
	mForward = rotationY.GetColumn(2);
	mRight = eyeDirection.Cross(up);

	// update player node
	mpNode->Local.SetTranslate(GetPosition());
	mpNode->Local.SetRotate(rotationYX);

	// update camera
	mspCamera->SetFrame(GetPosition(), eyeDirection, up, mRight);

	UpdateGunRotation(rotationY);

	DoShooting(eyeDirection);

	// move physics entity
	if (mJump)
	{
		mpPhysicsEntity->jump();
	}

	mpPhysicsEntity->setWalkDirection(PhysicsWorld::Convert(mMove));

	// Reset accumulators
	mMove = Vector3F::ZERO;
	mPitchIncrement = 0;
	mYawIncrement = 0;
	mJump = false;


	return true;
}

//----------------------------------------------------------------------------
void Player::Register(btDynamicsWorld* pPhysicsWorld)
{
	WIRE_ASSERT(pPhysicsWorld);
	mpPhysicsWorld = pPhysicsWorld;

	mpGhostObject = WIRE_NEW btPairCachingGhostObject();

	btConvexShape* pConvexShape = WIRE_NEW btCapsuleShape(mCharacterWidth, mCharacterHeight);
	mpGhostObject->setCollisionShape(pConvexShape);
	mpGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	// Add a reference to the controller in the physics object
	mpGhostObject->setUserPointer(this);

	// Create physics entity
	mpPhysicsEntity = WIRE_NEW btKinematicCharacterController(mpGhostObject, pConvexShape, mStepHeight);

	mpPhysicsWorld->addCollisionObject(mpGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | 
		btBroadphaseProxy::CharacterFilter | 
		btBroadphaseProxy::DefaultFilter);
	mpPhysicsWorld->addAction(mpPhysicsEntity);


	mpNode = DynamicCast<Node>(GetSceneObject());
	WIRE_ASSERT(mpNode);

	mpGun = mpNode->GetChildByName("Gun");

	// Set camera position
	Vector3F cameraPosition = mpNode->World.GetTranslate();
	mspCamera->SetFrame(cameraPosition, Vector3F(0, 0, -1), Vector3F::UNIT_Y, Vector3F::UNIT_X);

	// Set physics entity position
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(PhysicsWorld::Convert(mspCamera->GetLocation()));
	mpGhostObject->setWorldTransform(transform);
}

//----------------------------------------------------------------------------
void Player::SetMoveSpeed(Float moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

//----------------------------------------------------------------------------
void Player::MoveForward()
{
	mMove += mForward * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::MoveBackward()
{
	mMove -= mForward * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::StrafeLeft()
{
	mMove -= mRight * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::StrafeRight()
{
	mMove += mRight * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::Jump()
{
	mJump = true;
}

//----------------------------------------------------------------------------
void Player::Shoot()
{
	mShoot = true;
}

//----------------------------------------------------------------------------
void Player::LookAt(const Vector2F& rLookAt)
{
	mLookAt = rLookAt;

	if (rLookAt.X() > mLookUpDeadZone.X())
	{
		mYawIncrement -= mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}
	else if (rLookAt.X() < -mLookUpDeadZone.X())
	{
		mYawIncrement -= mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}

	if (rLookAt.Y() > mLookUpDeadZone.Y())
	{
		mPitchIncrement -= mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}
	else if (rLookAt.Y() < -mLookUpDeadZone.Y())
	{
		mPitchIncrement -= mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}
}

//----------------------------------------------------------------------------
Vector3F Player::GetPosition()
{
	btVector3 origin = mpGhostObject->getWorldTransform().getOrigin();
	return Vector3F(origin.x(), origin.y() + mHeadHeight, origin.z());
}

//----------------------------------------------------------------------------
void Player::UpdateGunRotation(Matrix3F& rRotation)
{
	if (mpGun == NULL)
	{
		return;
	}

	Float width = Application::GetApplication()->GetWidthF();
	Float height = Application::GetApplication()->GetHeightF();
	Vector2F cursorPosition(mLookAt.X()*2/width, mLookAt.Y()*2/height);

	Matrix4F projectionMatrix = mspCamera->GetProjectionMatrix();
	Vector3F pickDirection(-cursorPosition.X() / projectionMatrix(0, 0),
		cursorPosition.Y() / projectionMatrix(1, 1), 1);

	Vector3F tempUp(0, 1, 0);
	Vector3F right = pickDirection.Cross(tempUp);
	Vector3F up = right.Cross(pickDirection);
	pickDirection.Normalize();
	right.Normalize();
	up.Normalize();
	Matrix3F mat(-right, up, pickDirection, true);
	mpGun->Local.SetRotate(mat);
}

//----------------------------------------------------------------------------
void Player::DoShooting(const Vector3F& rDirection)
{
	// Remove previous ray
	Spatial* pRay = mpNode->GetChildByName("Ray");

	if (pRay) 
	{
		mpNode->DetachChild(pRay);
	}

	// If not shooting, exit
	if (!mShoot) 
	{
		return;
	}

	btVector3 rayStart = PhysicsWorld::Convert(GetPosition());
	btVector3 rayEnd = rayStart + PhysicsWorld::Convert(rDirection * mMaximumShootingDistance);

	btCollisionWorld::ClosestRayResultCallback hitCallback(rayStart, rayEnd);

	mpPhysicsWorld->rayTest(rayStart, rayEnd, hitCallback);
	if (hitCallback.hasHit()) 
	{
		Vector3F hitPoint = PhysicsWorld::Convert(hitCallback.m_hitPointWorld);
		CreateRay(GetPosition().Distance(hitPoint));

		ProbeRobot* pProbeRobotController = static_cast<ProbeRobot*>(hitCallback.m_collisionObject->getUserPointer());

		if (pProbeRobotController) 
		{
			pProbeRobotController->TakeDamage(5.0F);
		}
	}

	mShoot = false;
}

//----------------------------------------------------------------------------
void Player::CreateRay(Float size)
{
	RenderObject* pRenderObject = StandardMesh::CreateCylinder(5, 0.1F, size, 0, 4);
	Node* pRay = WIRE_NEW Node(pRenderObject);
	pRay->SetName("Ray");
	mpNode->AttachChild(pRay);
	mpNode->UpdateRS();

	Vector3F gunEnd = mpGun->Local.GetTranslate() + (mpGun->Local.GetRotate()
		* Vector3F(0, 0, 1.2F)) * (size * 0.5F);
	pRay->Local.SetTranslate(gunEnd);
	pRay->Local.SetRotate(mpGun->Local.GetRotate());
}
