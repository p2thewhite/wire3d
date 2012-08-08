#pragma once
#ifndef MATERIALFADER_H
#define MATERIALFADER_H

#include "WireController.h"
#include "WireSpatial.h"
#include "WireStateMaterial.h"

using namespace Wire;

class MaterialFader : public Controller
{
public:
	MaterialFader(Float duration, Bool fadeOut = true, Float delay = 0);

	virtual Bool Update(Double appTime);
	void DoEndingStep( Spatial* pSpatial, StateMaterial* pMaterialState );
	void DoFadeStep( StateMaterial* pMaterialState );

private:
	Float mTotalTime;
	Float mDelay;
	Float mTimeLeft;
	Bool mFadeOut;

};

#endif
