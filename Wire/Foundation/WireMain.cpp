#include "WireMain.h"

using namespace Wire;

Main::InitializerArray* Main::mpsInitializers = NULL;
Main::TerminatorArray* Main::mpsTerminators = NULL;

//----------------------------------------------------------------------------
void Main::AddInitializer(Initializer initializer)
{
	if (!mpsInitializers)
	{
		mpsInitializers = WIRE_NEW InitializerArray(10, 10);
	}

	mpsInitializers->Append(initializer);
}

//----------------------------------------------------------------------------
void Main::AddTerminator(Terminator terminator)
{
	if (!mpsTerminators)
	{
		mpsTerminators = WIRE_NEW TerminatorArray(10, 10);
	}

	mpsTerminators->Append(terminator);
}

//----------------------------------------------------------------------------
void Main::Initialize()
{
	if (mpsInitializers)
	{
		for (UInt i = 0; i < mpsInitializers->GetQuantity(); i++)
		{
			(*mpsInitializers)[i]();
		}
	}

	WIRE_DELETE mpsInitializers;
	mpsInitializers = NULL;
}

//----------------------------------------------------------------------------
void Main::Terminate()
{
	if (mpsTerminators)
	{
		for (UInt i = 0; i < mpsTerminators->GetQuantity(); i++)
		{
			(*mpsTerminators)[i]();
		}
	}

	WIRE_DELETE mpsTerminators;
	mpsTerminators = NULL;
}
