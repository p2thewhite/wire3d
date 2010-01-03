#pragma once
#ifndef WIREWIREFRAMESTATE_H
#define WIREWIREFRAMESTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ WireframeState : public GlobalState
{
	WIRE_DECLARE_INITIALIZE;
	WIRE_DECLARE_TERMINATE;

public:
	WireframeState();
	virtual ~WireframeState();

	virtual StateType GetStateType() const { return WIREFRAME; }

	Bool Enabled;         // default: false
};

WIRE_REGISTER_INITIALIZE(WireframeState);
WIRE_REGISTER_TERMINATE(WireframeState);
typedef Pointer<WireframeState> WireframeStatePtr;

}

#endif
