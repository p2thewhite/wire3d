#include "Sample8.h"

using namespace Wire;

WIRE_APPLICATION(Sample8);

//----------------------------------------------------------------------------
Sample8::Sample8()
	:
	mAngle(0.0F),
	mLastTime(0.0),
	mpCollisionConfiguration(NULL),
	mpDispatcher(NULL),
	mpOverlappingPairCache(NULL),
	mpSolver(NULL),
	mpDynamicsWorld(NULL)
{
}

//----------------------------------------------------------------------------
Bool Sample8::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	Geometry* pGeometry = StandardMesh::CreateCube8(4, false, 0.5F);
	pGeometry->World.SetScale(Vector3F(10.0F, 0.25F, 10.0F));
	pGeometry->World.SetTranslate(Vector3F(0, -4.125F, 0));
	mCubes.Append(pGeometry);

	Random rnd;
	for (Float y = -1.5F; y < 4.0F; y = y+1.0F)
	{
		for (Float x = -2.5F; x < 3.0F; x = x+1.0F)
		{
			Geometry* pGeometry = StandardMesh::CreateCube8(4, false, 0.5F);
			pGeometry->World.SetTranslate(Vector3F(x, y, rnd.GetFloat()));
			mCubes.Append(pGeometry);
		}
	}

	CreatePhysics();

	mspCamera = WIRE_NEW Camera;

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	UInt width = GetRenderer()->GetWidth();
	UInt height = GetRenderer()->GetHeight();
	Float fieldOfView = 45.0F;
	Float aspectRatio = static_cast<Float>(width)/static_cast<Float>(height);
	Float nearPlane = 0.1F;
	Float farPlane = 300.0F;
	mspCamera->SetFrustum(fieldOfView, aspectRatio, nearPlane, farPlane);

	return true;
}

//----------------------------------------------------------------------------
void Sample8::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	mAngle += static_cast<Float>(elapsedTime*0.5);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Float sinus = MathF::Sin(mAngle); 
	Float d = 15.0F + 5 * sinus;
	Float camY = -1.5F;
	Vector3F camPos(sinus*d, camY, MathF::Cos(mAngle)*d);
	mspCamera->LookAt(camPos, Vector3F(0, camY, 0), Vector3F::UNIT_Y);
	mCuller.SetCamera(mspCamera);

	UpdatePhysics(static_cast<btScalar>(elapsedTime));
	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);

	for (UInt i = 0; i < mCubes.GetQuantity(); i++)
	{
		Geometry* pGeometry = mCubes[i];
		WIRE_ASSERT(pGeometry);
		pGeometry->UpdateWorldBound();
		if (mCuller.IsVisible(pGeometry))
		{
			GetRenderer()->Draw(pGeometry);
		}
	}

	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void Sample8::OnTerminate()
{
	DestroyPhysics();
}

//----------------------------------------------------------------------------
void Sample8::CreatePhysics()
{
	///collision configuration contains default setup for memory,
	// collision setup. Advanced users can create their own configuration.
	mpCollisionConfiguration = WIRE_NEW btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you
	// can use a different dispatcher (see Extras/BulletMultiThreaded)
	mpDispatcher = WIRE_NEW btCollisionDispatcher(mpCollisionConfiguration);

	///the maximum size of the collision world. Make sure objects stay
	// within these boundaries
	///Don't make the world AABB size too large, it will harm simulation
	// quality and performance
	btVector3 worldAabbMin(-100, -100, -100);
	btVector3 worldAabbMax(100, 100, 100);
	Int	maxProxies = 1024;
	mpOverlappingPairCache = WIRE_NEW btAxisSweep3(worldAabbMin, worldAabbMax,
		maxProxies);

	///the default constraint solver. For parallel processing you can use
	// a different solver (see Extras/BulletMultiThreaded)
	mpSolver = WIRE_NEW btSequentialImpulseConstraintSolver;

	mpDynamicsWorld = WIRE_NEW btDiscreteDynamicsWorld(mpDispatcher,
		mpOverlappingPairCache, mpSolver, mpCollisionConfiguration);

	mpDynamicsWorld->setGravity(btVector3(0, -10, 0));

	///create a few basic rigid bodies

	{
		///Please don't make the box sizes larger then 1000: the collision
		// detection will be inaccurate.
		///See http://www.continuousphysics.com/Bullet/phpBB2/viewtopic.php?t=346

		btCollisionShape* pGroundShape = WIRE_NEW btBoxShape(btVector3(
			btScalar(5), btScalar(0.25F), btScalar(5)));

		//keep track of the shapes, we release memory at exit. make sure to
		// re-use collision shapes among rigid bodies whenever possible!
		mCollisionShapes.push_back(pGroundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		btVector3 origin = ToBtVector3(mCubes[0]->World.GetTranslate());
		groundTransform.setOrigin(origin);

		btScalar mass(0.0F);
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.0F);
		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
		{
			pGroundShape->calculateLocalInertia(mass, localInertia);
		}

		//using motionstate is recommended, it provides interpolation
		// capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* pMotionState = WIRE_NEW btDefaultMotionState(
			groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, pMotionState,
			pGroundShape, localInertia);
		btRigidBody* pBody = WIRE_NEW btRigidBody(rbInfo);

		//add the pBody to the dynamics world
		mpDynamicsWorld->addRigidBody(pBody);
	}

	for (UInt i = 1; i < mCubes.GetQuantity(); i++)
	{
		//create a dynamic rigidbody

		btCollisionShape* pColShape = WIRE_NEW btBoxShape(btVector3(0.5F,
			0.5F, 0.5F));
		//btCollisionShape* pColShape = WIRE_NEW btSphereShape(btScalar(1.));
		mCollisionShapes.push_back(pColShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.0F);

		//rigidbody is dynamic if and only if mass is non zero
		bool isDynamic = (mass != 0.0F);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
		{
			pColShape->calculateLocalInertia(mass, localInertia);
		}

		btVector3 origin = ToBtVector3(mCubes[i]->World.GetTranslate());
		startTransform.setOrigin(origin);

		//using motionstate is recommended, it provides interpolation
		//capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* pMotionState = WIRE_NEW btDefaultMotionState(
			startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, pMotionState,
			pColShape, localInertia);
		btRigidBody* body = WIRE_NEW btRigidBody(rbInfo);

		mpDynamicsWorld->addRigidBody(body);
	}
}

//----------------------------------------------------------------------------
void Sample8::DestroyPhysics()
{
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	for (Int i = mpDynamicsWorld->getNumCollisionObjects()-1; i >= 0; i--)
	{
		btCollisionObject* pObj = mpDynamicsWorld->
			getCollisionObjectArray()[i];
		btRigidBody* pBody = btRigidBody::upcast(pObj);
		if (pBody && pBody->getMotionState())
		{
			WIRE_DELETE pBody->getMotionState();
		}

		mpDynamicsWorld->removeCollisionObject(pObj);
		WIRE_DELETE pObj;
	}

	//delete collision shapes
	for (Int j = 0; j < mCollisionShapes.size(); j++)
	{
		btCollisionShape* pShape = mCollisionShapes[j];
		mCollisionShapes[j] = 0;
		WIRE_DELETE pShape;
	}

	WIRE_DELETE mpDynamicsWorld;
	WIRE_DELETE mpSolver;
	WIRE_DELETE mpOverlappingPairCache;
	WIRE_DELETE mpDispatcher;
	WIRE_DELETE mpCollisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the
	//array goes out of scope
	mCollisionShapes.clear();
}

//----------------------------------------------------------------------------
void Sample8::UpdatePhysics(btScalar elapsedTime)
{
	mpDynamicsWorld->stepSimulation(elapsedTime, 10);

	QuaternionF quat;
	Matrix3F mat;

	const btCollisionObjectArray& objectArray = mpDynamicsWorld->
		getCollisionObjectArray();

	for (Int j = mpDynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
	{
		btCollisionObject* obj = objectArray[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			Vector3F pos = Vector3F(Float(trans.getOrigin().getX()), Float(
				trans.getOrigin().getY()), Float(trans.getOrigin().getZ()));

			QuaternionF quat;
			ToWireQuaternion(trans.getRotation(), quat);
			quat.ToRotationMatrix(mat);
			mCubes[j]->World.SetTranslate(pos);
			mCubes[j]->World.SetRotate(mat);
		}
	}
}
