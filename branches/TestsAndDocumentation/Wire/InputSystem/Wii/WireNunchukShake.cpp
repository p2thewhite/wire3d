// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNunchukShake.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, NunchukShake, Shake);

NunchukShake::NunchukShake(const InputDevice* pParent)
	: Shake(pParent)
{
}

NunchukShake::~NunchukShake()
{
}

Float NunchukShake::GetX() const
{
	return 0;
}

Float NunchukShake::GetY() const
{
	return 0;
}

Float NunchukShake::GetZ() const
{
	return 0;
}
