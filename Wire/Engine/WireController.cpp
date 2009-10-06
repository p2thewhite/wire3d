#include "WireController.h"

#include "../Foundation/WireMath.h"

using namespace Wire;

//----------------------------------------------------------------------------
Controller::Controller()
{
	mpObject = NULL;
	mLastAppTime = -MathD::MAX_REAL;
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
