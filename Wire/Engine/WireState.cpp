// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, State, Object);

StatePtr State::Default[MAX_STATE_TYPE];

//----------------------------------------------------------------------------
State::State()
{
}

//----------------------------------------------------------------------------
State::~State()
{
}