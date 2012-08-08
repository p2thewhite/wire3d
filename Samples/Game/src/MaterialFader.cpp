#include "MaterialFader.h"

using namespace Wire;

//----------------------------------------------------------------------------
MaterialFader::MaterialFader(Float duration, Bool fadeOut, Float delay)
	:
	mTotalTime(duration), 
	mDelay(delay),
	mTimeLeft(duration),
	mFadeOut(fadeOut)
{
	WIRE_ASSERT(duration > 0);
	mTimeLeft = duration;
}

//----------------------------------------------------------------------------
Bool MaterialFader::Update(Double appTime)
{
	Spatial* pSpatial = DynamicCast<Spatial>(mpObject);

	if (!pSpatial || (appTime == -MathD::MAX_REAL))
	{
		return false;
	}

	StateMaterial* pMaterialState = DynamicCast<StateMaterial>(pSpatial->GetState(State::MATERIAL));
	if (!pMaterialState)
	{
		return false;
	}

	Float deltaTime = static_cast<Float>(appTime - mLastApplicationTime);
	if (mLastApplicationTime == -MathD::MAX_REAL)
	{
		deltaTime = 0.0F;
	}

	mLastApplicationTime = appTime;

	if (mDelay > 0.0F)
	{
		mDelay -= deltaTime;
	}
	else if (mTimeLeft > 0)
	{
		mTimeLeft -= deltaTime;
		pSpatial->Culling = Spatial::CULL_DYNAMIC;
		DoFadeStep(pMaterialState);
	}
	else
	{
		DoEndingStep(pSpatial, pMaterialState);
		pSpatial->DetachController(this);
	}

	return true;
}

//----------------------------------------------------------------------------
void MaterialFader::DoFadeStep(StateMaterial* pMaterialState)
{
	pMaterialState->Ambient.A() = (mTimeLeft / mTotalTime) - ((mFadeOut) ? 0 : 1);
}

//----------------------------------------------------------------------------
void MaterialFader::DoEndingStep(Spatial* pSpatial, StateMaterial* pMaterialState)
{
	if (mFadeOut)
	{
		pSpatial->Culling = Spatial::CULL_ALWAYS;
		pMaterialState->Ambient.A() = 0.0F;
	}
	else
	{
		pMaterialState->Ambient.A() = 1.0F;
	}
}
