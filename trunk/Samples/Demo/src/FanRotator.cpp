#include "FanRotator.h"

#include "WireSpatial.h"

using namespace Wire;

//----------------------------------------------------------------------------
FanRotator::FanRotator(Float speed)
	:
	mSpeed(speed),
	mAngle(0)
{
}

//----------------------------------------------------------------------------
Bool FanRotator::Update(Double appTime)
{
	Spatial* pSpatial = DynamicCast<Spatial>(mpObject);
	Double deltaTime = appTime - mLastAppTime;
	
	if (!pSpatial || !Controller::Update(appTime))
	{
		return false;
	}

	Matrix34F rotate(Vector3F::UNIT_Y, mAngle);
	pSpatial->Local.SetRotate(rotate);
	
	mAngle += static_cast<Float>(deltaTime) * mSpeed;
	return true;
}
