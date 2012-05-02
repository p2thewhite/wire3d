// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Buffer, Object);

//----------------------------------------------------------------------------
Buffer::Buffer(UsageType usage)
	:
	mUsage(usage)
{
}

//----------------------------------------------------------------------------
Buffer::~Buffer()
{
}