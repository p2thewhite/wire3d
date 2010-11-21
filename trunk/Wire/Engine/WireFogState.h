#pragma once
#ifndef WIREFOGSTATE_H
#define WIREFOGSTATE_H

#include "WireColorRGB.h"
#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ FogState : public GlobalState
{
	WIRE_DECLARE_RTTI;

public:
	FogState();
	virtual ~FogState();

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

typedef Pointer<FogState> FogStatePtr;

}

#endif
