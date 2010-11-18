#pragma once
#ifndef WIREWIREFRAMESTATE_H
#define WIREWIREFRAMESTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ WireframeState : public GlobalState
{
	WIRE_DECLARE_RTTI;

public:
	WireframeState();
	virtual ~WireframeState();

	virtual StateType GetStateType() const { return WIREFRAME; }

	Bool Enabled;         // default: false
};

typedef Pointer<WireframeState> WireframeStatePtr;

}

#endif
