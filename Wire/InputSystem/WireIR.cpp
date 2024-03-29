// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2014. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireIR.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, IR, InputCapability);

IR::IR(const InputDevice* pParent)
	: InputCapability(pParent)
{
}

IR::~IR()
{
}
