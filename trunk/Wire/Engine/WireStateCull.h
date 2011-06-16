// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRECULLSTATE_H
#define WIRECULLSTATE_H

#include "WireState.h"

namespace Wire
{

class StateCull : public State
{
	WIRE_DECLARE_RTTI;

public:
	StateCull();
	virtual ~StateCull();

	virtual StateType GetStateType() const { return CULL; }

	enum CullMode
	{
		CM_OFF,		// do not cull any triangles
		CM_FRONT,	// cull front-facing triangles
		CM_BACK,	// cull back-facing triangles
		CM_QUANTITY
	};

	Bool Enabled;         // default: true
	CullMode CullFace;    // default: CM_BACK
};

typedef Pointer<StateCull> StateCullPtr;

}

#endif
