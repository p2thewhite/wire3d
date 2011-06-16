// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIREFRAMESTATE_H
#define WIREWIREFRAMESTATE_H

#include "WireState.h"

namespace Wire
{

class StateWireframe : public State
{
	WIRE_DECLARE_RTTI;

public:
	StateWireframe();
	virtual ~StateWireframe();

	inline virtual StateType GetStateType() const { return WIREFRAME; }

	Bool Enabled;         // default: false
};

typedef Pointer<StateWireframe> StateWireframePtr;

}

#endif
