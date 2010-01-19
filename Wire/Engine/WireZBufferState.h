#pragma once
#ifndef WIREZBUFFERSTATE_H
#define WIREZBUFFERSTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ ZBufferState : public GlobalState
{
	WIRE_DECLARE_RTTI;
	WIRE_DECLARE_INITIALIZE;
	WIRE_DECLARE_TERMINATE;

public:
	ZBufferState();
	virtual ~ZBufferState();

	virtual StateType GetStateType() const { return ZBUFFER; }

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

WIRE_REGISTER_INITIALIZE(ZBufferState);
WIRE_REGISTER_TERMINATE(ZBufferState);
typedef Pointer<ZBufferState> ZBufferStatePtr;

}

#endif
