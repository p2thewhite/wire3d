#pragma once
#ifndef WIREEFFECT_H
#define WIREEFFECT_H

#include "WireObject.h"

namespace Wire
{

class Effect : public Object
{
	WIRE_DECLARE_RTTI;

public:
	// abstract base class
	virtual ~Effect();

protected:
	Effect();
};

typedef Pointer<Effect> EffectPtr;

}

#endif
