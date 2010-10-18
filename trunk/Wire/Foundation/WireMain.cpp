#include "WireMain.h"

using namespace Wire;

Main::InitializerArray* Main::smpInitializers = NULL;
Main::TerminatorArray* Main::smpTerminators = NULL;

//----------------------------------------------------------------------------
void Main::AddInitializer(Initializer initializer)
{
	if (!smpInitializers)
	{
		smpInitializers = WIRE_NEW InitializerArray(10, 10);
	}

	smpInitializers->Append(initializer);
}

//----------------------------------------------------------------------------
void Main::AddTerminator(Terminator terminator)
{
	if (!smpTerminators)
	{
		smpTerminators = WIRE_NEW TerminatorArray(10, 10);
	}

	smpTerminators->Append(terminator);
}

//----------------------------------------------------------------------------
void Main::Initialize()
{
	if (smpInitializers)
	{
		for (UInt i = 0; i < smpInitializers->GetQuantity(); i++)
		{
			(*smpInitializers)[i]();
		}
	}

	WIRE_DELETE smpInitializers;
	smpInitializers = NULL;
}

//----------------------------------------------------------------------------
void Main::Terminate()
{
	if (smpTerminators)
	{
		for (UInt i = 0; i < smpTerminators->GetQuantity(); i++)
		{
			(*smpTerminators)[i]();
		}
	}

	WIRE_DELETE smpTerminators;
	smpTerminators = NULL;
}
