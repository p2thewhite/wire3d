#pragma once
#ifndef WIRECULLSTATE_H
#define WIRECULLSTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ CullState : public GlobalState
{
	WIRE_DECLARE_RTTI;

public:
	CullState();
	virtual ~CullState();

	virtual StateType GetStateType() const { return CULL; }

	enum CullMode
	{
		CM_OFF,		// do not cull any triangles
		CM_FRONT,	// cull front-facing triangles
		CM_BACK,	// cull back-facing triangles
		CM_QUANTITY
	};

	Bool Enabled;         // default: true
	CullMode CullFace;    // default: CM_BACK
};

typedef Pointer<CullState> CullStatePtr;

}

#endif
