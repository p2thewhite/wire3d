#pragma once
#ifndef WIREFOGSTATE_H
#define WIREFOGSTATE_H

#include "WireGlobalState.h"
#include "../Foundation/WireColorRGB.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ FogState : public GlobalState
{
	WIRE_DECLARE_INITIALIZE;
	WIRE_DECLARE_TERMINATE;

public:
	FogState();
	virtual ~FogState();

	virtual StateType GetStateType() const { return FOG; }

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

WIRE_REGISTER_INITIALIZE(FogState);
WIRE_REGISTER_TERMINATE(FogState);
typedef Pointer<FogState> FogStatePtr;

}

#endif
