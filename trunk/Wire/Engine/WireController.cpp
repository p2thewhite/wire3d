// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireController.h"

#include "WireMath.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Controller, Object);

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
