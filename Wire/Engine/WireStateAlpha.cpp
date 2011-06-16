// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireStateAlpha.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateAlpha, State);

//----------------------------------------------------------------------------
StateAlpha::StateAlpha()
	:
	BlendEnabled(false),
	SrcBlend(SBM_SRC_ALPHA),
	DstBlend(DBM_ONE_MINUS_SRC_ALPHA)
{
}

//----------------------------------------------------------------------------
StateAlpha::~StateAlpha()
{
}
