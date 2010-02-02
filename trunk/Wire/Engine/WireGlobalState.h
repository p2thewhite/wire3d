#pragma once
#ifndef WIREGLOBALSTATE_H
#define WIREGLOBALSTATE_H

#include "WireMain.h"
#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ GlobalState : public Object
{
	WIRE_DECLARE_RTTI;

public:
	// abstract base class
	virtual ~GlobalState();

	// supported global states
	enum StateType
	{
		ALPHA,
		CULL,
		FOG,
		WIREFRAME,
		ZBUFFER,
		MAX_STATE_TYPE
	};

	virtual StateType GetStateType() const = 0;

	// default states
	static Pointer<GlobalState> Default[MAX_STATE_TYPE];

protected:
	GlobalState();
};

typedef Pointer<GlobalState> GlobalStatePtr;

}

#endif
