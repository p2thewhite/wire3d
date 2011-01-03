#pragma once
#ifndef WIREGLOBALSTATE_H
#define WIREGLOBALSTATE_H

#include "WireMain.h"
#include "WireObject.h"

namespace Wire
{

class StateGlobal : public Object
{
	WIRE_DECLARE_RTTI;

public:
	// abstract base class
	virtual ~StateGlobal();

	// supported global states
	enum StateType
	{
		ALPHA,
		CULL,
		FOG,
		MATERIAL,
		WIREFRAME,
		ZBUFFER,
		MAX_STATE_TYPE
	};

	virtual StateType GetStateType() const = 0;

protected:
	StateGlobal();
};

typedef Pointer<StateGlobal> StateGlobalPtr;

}

#endif
