#include "CubeController.h"

#include "WireGeometry.h"
#include "WireMath.h"

//----------------------------------------------------------------------------
CubeController::CubeController()
	:
	mAngle(0.0F)
{
}

//----------------------------------------------------------------------------
CubeController::~CubeController()
{
}

//----------------------------------------------------------------------------
Bool CubeController::Update(Double appTime)
{
	Double lastTime = mLastAppTime;

	if (!Controller::Update(appTime))
	{
		// update has already been performed at 'appTime'.
		return false;
	}

	Geometry* pCube = StaticCast<Geometry>(mpObject);

	Double elapsed = appTime - lastTime;
	mAngle += static_cast<Float>(elapsed) * 2.0F;
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	Matrix34F model(Vector3F(1, 1, 1), mAngle);
	pCube->Local.SetRotate(model);
	pCube->Local.SetTranslate(Vector3F(2.5F, 0.0F, 0.0F));

	return true;
}
