#include "WireMain.h"

using namespace Wire;

Main::InitializerArray* Main::smpInitializers = NULL;

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
void Main::Initialize()
{
	if (smpInitializers)
	{
		for (Int i = 0; i < smpInitializers->GetQuantity(); i++)
		{
			(*smpInitializers)[i]();
		}
	}

	WIRE_DELETE smpInitializers;
	smpInitializers = 0;
}
