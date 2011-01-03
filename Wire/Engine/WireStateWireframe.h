#pragma once
#ifndef WIREWIREFRAMESTATE_H
#define WIREWIREFRAMESTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class StateWireframe : public StateGlobal
{
	WIRE_DECLARE_RTTI;

public:
	StateWireframe();
	virtual ~StateWireframe();

	inline virtual StateType GetStateType() const { return WIREFRAME; }

	Bool Enabled;         // default: false
};

typedef Pointer<StateWireframe> StateWireframePtr;

}

#endif
