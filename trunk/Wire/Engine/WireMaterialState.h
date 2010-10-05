#pragma once
#ifndef WIREMATERIALSTATE_H
#define WIREMATERIALSTATE_H

#include "WireColorRGBA.h"
#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ MaterialState : public GlobalState
{
	WIRE_DECLARE_RTTI;
	WIRE_DECLARE_INITIALIZE;
	WIRE_DECLARE_TERMINATE;

public:
	MaterialState();
	virtual ~MaterialState();

	virtual StateType GetStateType() const { return MATERIAL; }

    ColorRGBA Ambient;   // default:  ColorRGBA(0.2, 0.2, 0.2, 1)
};

WIRE_REGISTER_INITIALIZE(MaterialState);
WIRE_REGISTER_TERMINATE(MaterialState);
typedef Pointer<MaterialState> MaterialStatePtr;

}

#endif
