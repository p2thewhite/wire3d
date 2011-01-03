#pragma once
#ifndef WIREZBUFFERSTATE_H
#define WIREZBUFFERSTATE_H

#include "WireState.h"

namespace Wire
{

class StateZBuffer : public State
{
	WIRE_DECLARE_RTTI;

public:
	StateZBuffer();
	virtual ~StateZBuffer();

	inline virtual StateType GetStateType() const { return ZBUFFER; }

	enum CompareFunction
	{
		CF_NEVER,
		CF_LESS,
		CF_EQUAL,
		CF_LEQUAL,
		CF_GREATER,
		CF_NOTEQUAL,
		CF_GEQUAL,
		CF_ALWAYS,
		CF_QUANTITY
	};

	Bool Enabled;				// default: true
	Bool Writable;				// default: true
	CompareFunction Compare;	// default: CF_LEQUAL
};

typedef Pointer<StateZBuffer> StateZBufferPtr;

}

#endif
