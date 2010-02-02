#include "WireController.h"

#include "WireMath.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Controller, Object);

//----------------------------------------------------------------------------
Controller::Controller()
{
	mpObject = NULL;
	mLastAppTime = 0;
}

//----------------------------------------------------------------------------
Controller::~Controller()
{
}

//----------------------------------------------------------------------------
Bool Controller::Update(Double appTime)
{
	if (appTime == -MathD::MAX_REAL || appTime != mLastAppTime)
	{
		mLastAppTime = appTime;
		return true;
	}

	return false;
}
