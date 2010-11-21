#pragma once
#ifndef WIREZBUFFERSTATE_H
#define WIREZBUFFERSTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ ZBufferState : public GlobalState
{
	WIRE_DECLARE_RTTI;

public:
	ZBufferState();
	virtual ~ZBufferState();

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

typedef Pointer<ZBufferState> ZBufferStatePtr;

}

#endif
