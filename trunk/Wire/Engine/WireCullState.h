#pragma once
#ifndef WIRECULLSTATE_H
#define WIRECULLSTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ CullState : public GlobalState
{
	WIRE_DECLARE_INITIALIZE;
	WIRE_DECLARE_TERMINATE;

public:
	CullState();
	virtual ~CullState();

	virtual StateType GetStateType() const { return CULL; }

	enum CullMode
	{
		CT_FRONT,  // cull front-facing triangles
		CT_BACK,   // cull back-facing triangles
		CT_QUANTITY
	};

	Bool Enabled;         // default: true
	CullMode CullFace;    // default: CT_BACK
};

WIRE_REGISTER_INITIALIZE(CullState);
WIRE_REGISTER_TERMINATE(CullState);
typedef Pointer<CullState> CullStatePtr;

}

#endif
