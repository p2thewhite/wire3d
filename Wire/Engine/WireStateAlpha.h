#pragma once
#ifndef WIREALPHASTATE_H
#define WIREALPHASTATE_H

#include "WireState.h"

namespace Wire
{

class StateAlpha : public State
{
	WIRE_DECLARE_RTTI;

public:
	enum SrcBlendMode
	{
		SBM_ZERO,
		SBM_ONE,
		SBM_DST_COLOR,
		SBM_ONE_MINUS_DST_COLOR,
		SBM_SRC_ALPHA,
		SBM_ONE_MINUS_SRC_ALPHA,
		SBM_DST_ALPHA,
		SBM_ONE_MINUS_DST_ALPHA,
		SBM_QUANTITY
	};

	enum DstBlendMode
	{
		DBM_ZERO,
		DBM_ONE,
		DBM_SRC_COLOR,
		DBM_ONE_MINUS_SRC_COLOR,
		DBM_SRC_ALPHA,
		DBM_ONE_MINUS_SRC_ALPHA,
		DBM_DST_ALPHA,
		DBM_ONE_MINUS_DST_ALPHA,
		DBM_QUANTITY
	};

	StateAlpha();
	virtual ~StateAlpha();

	inline virtual StateType GetStateType() const { return ALPHA; }

	Bool BlendEnabled;      // default: false
	SrcBlendMode SrcBlend;  // default: SBM_SRC_ALPHA
	DstBlendMode DstBlend;  // default: DBM_ONE_MINUS_SRC_ALPHA
};

typedef Pointer<StateAlpha> StateAlphaPtr;

}

#endif
