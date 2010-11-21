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

public:
	MaterialState();
	virtual ~MaterialState();

	inline virtual StateType GetStateType() const { return MATERIAL; }

    ColorRGBA Ambient;   // default:  ColorRGBA(0.2, 0.2, 0.2, 1)
};

typedef Pointer<MaterialState> MaterialStatePtr;

}

#endif
