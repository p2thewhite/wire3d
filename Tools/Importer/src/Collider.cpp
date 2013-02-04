#include "Collider.h"

#include "BulletUtils.h"
#include "WireSpatial.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Collider, Controller);

//----------------------------------------------------------------------------
Collider::Collider(btCollisionShape* pShape, btDefaultMotionState*
	pMotionState)
	:
	mpShape(pShape),
	mpMotionState(pMotionState)
{
}

//----------------------------------------------------------------------------
Collider::~Collider()
{
}

//----------------------------------------------------------------------------
Vector3F Collider::GetWorldTranslate()
{
	if (!mpMotionState)
	{
		return Vector3F::ZERO;
	}

	btTransform trafo;
	mpMotionState->getWorldTransform(trafo);
	return BulletUtils::Convert(trafo.getOrigin());
}
