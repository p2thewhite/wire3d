// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireMain.h"

using namespace Wire;

Main::InitializerArray* Main::s_pInitializers = NULL;
Main::TerminatorArray* Main::s_pTerminators = NULL;

//----------------------------------------------------------------------------
void Main::AddInitializer(Initializer initializer)
{
	if (!s_pInitializers)
	{
		s_pInitializers = WIRE_NEW InitializerArray(10, 10);
	}

	s_pInitializers->Append(initializer);
}

//----------------------------------------------------------------------------
void Main::AddTerminator(Terminator terminator)
{
	if (!s_pTerminators)
	{
		s_pTerminators = WIRE_NEW TerminatorArray(10, 10);
	}

	s_pTerminators->Append(terminator);
}

//----------------------------------------------------------------------------
void Main::Initialize()
{
	if (s_pInitializers)
	{
		for (UInt i = 0; i < s_pInitializers->GetQuantity(); i++)
		{
			(*s_pInitializers)[i]();
		}
	}

	WIRE_DELETE s_pInitializers;
	s_pInitializers = NULL;
}

//----------------------------------------------------------------------------
void Main::Terminate()
{
	if (s_pTerminators)
	{
		for (UInt i = 0; i < s_pTerminators->GetQuantity(); i++)
		{
			(*s_pTerminators)[i]();
		}
	}

	WIRE_DELETE s_pTerminators;
	s_pTerminators = NULL;
}
