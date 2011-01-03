#pragma once
#ifndef WIREMATERIALSTATE_H
#define WIREMATERIALSTATE_H

#include "WireColorRGBA.h"
#include "WireState.h"

namespace Wire
{

class StateMaterial : public State
{
	WIRE_DECLARE_RTTI;

public:
	StateMaterial();
	virtual ~StateMaterial();

	inline virtual StateType GetStateType() const { return MATERIAL; }

    ColorRGBA Ambient;   // default:  ColorRGBA(0.2, 0.2, 0.2, 1)
};

typedef Pointer<StateMaterial> StateMaterialPtr;

}

#endif
