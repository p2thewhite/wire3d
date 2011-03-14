#include "Sample8.h"

using namespace Wire;

WIRE_APPLICATION(Sample8);

//----------------------------------------------------------------------------
Sample8::Sample8()
	:
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
	pGeometry->World.SetTranslate(Vector3F(0, -2.5F, 0));
	mCubes.Append(pGeometry);

	Random rnd;
	for (Float y = -1.5F; y < 2.0F; y = y+1.0F)
	{
		for (Float x = -1.5F; x < 2.0F; x = x+1.0F)
		{
			Geometry* pGeometry = StandardMesh::CreateCube8(4, false, 0.5F);
			pGeometry->World.SetTranslate(Vector3F(x, y, rnd.GetFloat()));
			mCubes.Append(pGeometry);
		}
	}

	CreatePhysics();

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, 0.0F, 15.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	UInt width = GetRenderer()->GetWidth();
	UInt height = GetRenderer()->GetHeight();
	Float fieldOfView = 45.0F;
	Float aspectRatio = static_cast<Float>(width)/static_cast<Float>(height);
	Float nearPlane = 0.1F;
	Float farPlane = 300.0F;
	mspCamera->SetFrustum(fieldOfView, aspectRatio, nearPlane, farPlane);
	mCuller.SetCamera(mspCamera);

	return true;
}

//----------------------------------------------------------------------------
void Sample8::OnIdle()
{
	UpdatePhysics();
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
	// Destroy Physics
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
	int	maxProxies = 1024;
	mpOverlappingPairCache = WIRE_NEW btAxisSweep3(worldAabbMin, worldAabbMax,
		maxProxies);

	///the default constraint solver. For parallel processing you can use
	// a different solver (see Extras/BulletMultiThreaded)
	mpSolver = WIRE_NEW btSequentialImpulseConstraintSolver;

	mpDynamicsWorld = WIRE_NEW btDiscreteDynamicsWorld(mpDispatcher,
		mpOverlappingPairCache, mpSolver, mpCollisionConfiguration);

	mpDynamicsWorld->setGravity(btVector3(0, -9, 0));

	///create a few basic rigid bodies
	btCollisionShape* pGroundShape = WIRE_NEW btBoxShape(btVector3(btScalar(5),
		btScalar(0.2f),btScalar(5)));

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	mCollisionShapes.push_back(pGroundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	btVector3 origin = ToBtVector3(mCubes[0]->World.GetTranslate());
	groundTransform.setOrigin(origin);

	{
		btScalar mass(0.);
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0,0,0);
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

		btCollisionShape* pColShape = WIRE_NEW btBoxShape(btVector3(0.5F, 0.5F, 0.5F));
		//btCollisionShape* pColShape = WIRE_NEW btSphereShape(btScalar(1.));
		mCollisionShapes.push_back(pColShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
		{
			pColShape->calculateLocalInertia(mass,localInertia);
		}

		startTransform.setOrigin(ToBtVector3(mCubes[i]->World.GetTranslate()));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* pMotionState = WIRE_NEW btDefaultMotionState(
			startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, pMotionState,
			pColShape,localInertia);
		btRigidBody* body = WIRE_NEW btRigidBody(rbInfo);

		mpDynamicsWorld->addRigidBody(body);
	}
}

//----------------------------------------------------------------------------
void Sample8::UpdatePhysics()
{
	mpDynamicsWorld->stepSimulation(1.F/60.F, 10);

	QuaternionF quat;
	Matrix3F mat;

	//print positions of all objects
	for (int j = mpDynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
	{
		btCollisionObject* obj = mpDynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			Vector3F pos = Vector3F(float(trans.getOrigin().getX()),
				float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));

			QuaternionF quat;
			ToWireQuaternion(trans.getRotation(), quat);
			quat.ToRotationMatrix(mat);
			mCubes[j]->World.SetTranslate(pos);
			mCubes[j]->World.SetRotate(mat);
		}
	}
}

//----------------------------------------------------------------------------
