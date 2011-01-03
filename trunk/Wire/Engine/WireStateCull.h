#pragma once
#ifndef WIRECULLSTATE_H
#define WIRECULLSTATE_H

#include "WireGlobalState.h"

namespace Wire
{

class StateCull : public StateGlobal
{
	WIRE_DECLARE_RTTI;

public:
	StateCull();
	virtual ~StateCull();

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

typedef Pointer<StateCull> StateCullPtr;

}

#endif
