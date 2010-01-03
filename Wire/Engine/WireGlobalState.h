#pragma once
#ifndef WIREGLOBALSTATE_H
#define WIREGLOBALSTATE_H

#include "WireObject.h"
#include "../Foundation/WireMain.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ GlobalState : public Object
{

public:
	// abstract base class
	virtual ~GlobalState();

	// supported global states
	enum StateType
	{
		ALPHA,
		CULL,
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
