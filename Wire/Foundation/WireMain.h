// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREMAIN_H
#define WIREMAIN_H

#include "WireTArray.h"

namespace Wire
{

class Main
{

public:
	typedef void (*Initializer)();
	typedef TArray<Initializer> InitializerArray;
	static void AddInitializer(Initializer initializer);
	static void Initialize();

	typedef void (*Terminator)();
	typedef TArray<Terminator> TerminatorArray;
	static void AddTerminator(Terminator terminator);
	static void Terminate();

private:
	static InitializerArray* s_pInitializers;
	static TerminatorArray* s_pTerminators;
};

}

//----------------------------------------------------------------------------
#define WIRE_DECLARE_INITIALIZE                                              \
public:                                                                      \
	static Bool RegisterInitialize();                                        \
	static void Initialize();                                                \
private:                                                                     \
	static Bool s_InitializeRegistered

//----------------------------------------------------------------------------
#define WIRE_DECLARE_TERMINATE                                               \
public:                                                                      \
	static Bool RegisterTerminate();                                         \
	static void Terminate();                                                 \
private:                                                                     \
	static Bool s_TerminateRegistered

//----------------------------------------------------------------------------
#define WIRE_REGISTER_INITIALIZE(classname)                                  \
static Bool gs_InitializeRegistered##classname =                             \
	classname::RegisterInitialize()

//----------------------------------------------------------------------------
#define WIRE_REGISTER_TERMINATE(classname)                                   \
static Bool gs_TerminateRegistered##classname =                              \
	classname::RegisterTerminate()

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_INITIALIZE(classname)                                 \
Bool classname::s_InitializeRegistered = false;                              \
Bool classname::RegisterInitialize()                                         \
{                                                                            \
	if (!s_InitializeRegistered)                                             \
	{                                                                        \
		Main::AddInitializer(classname::Initialize);                         \
		s_InitializeRegistered = true;                                       \
	}                                                                        \
                                                                             \
	return s_InitializeRegistered;                                           \
}

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_TERMINATE(classname)                                  \
Bool classname::s_TerminateRegistered = false;                               \
Bool classname::RegisterTerminate()                                          \
{                                                                            \
	if (!s_TerminateRegistered)                                              \
	{                                                                        \
		Main::AddTerminator(classname::Terminate);                           \
		s_TerminateRegistered = true;                                        \
	}                                                                        \
	                                                                         \
	return s_TerminateRegistered;                                            \
}

//----------------------------------------------------------------------------
#define WIRE_APPLICATION(classname)                                          \
WIRE_IMPLEMENT_INITIALIZE(classname);                                        \
                                                                             \
void classname::Initialize()                                                 \
{                                                                            \
	SetApplication(WIRE_NEW classname);                                      \
}                                                                            \
                                                                             \
Int main(Int argc, Char** argv)                                              \
{                                                                            \
	Main::Initialize();                                                      \
                                                                             \
	Application* pApplication = Application::GetApplication();               \
	if (pApplication)                                                        \
	{                                                                        \
		pApplication->Main(argc, argv);                                      \
		                                                                     \
		WIRE_DELETE pApplication;                                            \
		Application::SetApplication(NULL);                                   \
	}                                                                        \
                                                                             \
	Main::Terminate();                                                       \
                                                                             \
	return 0;                                                                \
}                                                                            \

#endif
