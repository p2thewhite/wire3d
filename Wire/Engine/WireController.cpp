// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
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
	:
	mpSceneObject(NULL),
	mLastApplicationTime(-MathD::MAX_REAL)
{
}

//----------------------------------------------------------------------------
Controller::~Controller()
{
}

//----------------------------------------------------------------------------
Bool Controller::Update(Double appTime)
{
	if (appTime == -MathD::MAX_REAL || appTime != mLastApplicationTime)
	{
		mLastApplicationTime = appTime;
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------
Bool Controller::OnGetVisibleUpdate(const Camera* /*pCamera*/)
{
	return false;
}
