#pragma once
#ifndef WIREFOGSTATE_H
#define WIREFOGSTATE_H

#include "WireColorRGB.h"
#include "WireGlobalState.h"

namespace Wire
{

class StateFog : public StateGlobal
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
