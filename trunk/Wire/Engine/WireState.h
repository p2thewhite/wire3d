// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGLOBALSTATE_H
#define WIREGLOBALSTATE_H

#include "WireMain.h"
#include "WireObject.h"

namespace Wire
{

class State : public Object
{
	WIRE_DECLARE_RTTI;

public:
	// abstract base class
	virtual ~State();

	// supported global states
	enum StateType
	{
		ALPHA,
		CULL,
		FOG,
		MATERIAL,
		WIREFRAME,
		ZBUFFER,
		MAX_STATE_TYPE
	};

	virtual StateType GetStateType() const = 0;

protected:
	State();
};

typedef Pointer<State> StatePtr;

}

#endif
