// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREFOGSTATE_H
#define WIREFOGSTATE_H

#include "WireColorRGB.h"
#include "WireState.h"

namespace Wire
{

class StateFog : public State
{
	WIRE_DECLARE_RTTI;

public:
	StateFog();
	virtual ~StateFog();

	inline virtual StateType GetStateType() const { return FOG; }

	enum DensityFunction
	{
		DF_LINEAR,
		DF_EXP,
		DF_EXPSQR,
		DF_QUANTITY
	};

	Bool Enabled;					// default: false
	ColorRGB Color;					// default: ColorRGB::BLACK
	DensityFunction DensityFunc;	// default: DF_LINEAR
	Float Start;					// default: 0.0F
	Float End;						// default: 1.0F
};

typedef Pointer<StateFog> StateFogPtr;

}

#endif
