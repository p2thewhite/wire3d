// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREEFFECT_H
#define WIREEFFECT_H

#include "WireObject.h"

namespace Wire
{

class Effect : public Object
{
	WIRE_DECLARE_RTTI;

public:
	// abstract base class
	virtual ~Effect();

protected:
	Effect();
};

typedef Pointer<Effect> EffectPtr;

}

#endif
