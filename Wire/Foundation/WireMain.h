// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
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
	static InitializerArray* smpInitializers;
	static TerminatorArray* smpTerminators;
};

}

//----------------------------------------------------------------------------
#define WIRE_DECLARE_INITIALIZE                                              \
public:                                                                      \
	static Bool RegisterInitialize();                                        \
	static void Initialize();                                                \
private:                                                                     \
	static Bool msInitializeRegistered

//----------------------------------------------------------------------------
#define WIRE_DECLARE_TERMINATE                                               \
public:                                                                      \
	static Bool RegisterTerminate();                                         \
	static void Terminate();                                                 \
private:                                                                     \
	static Bool msTerminateRegistered

//----------------------------------------------------------------------------
#define WIRE_REGISTER_INITIALIZE(classname)                                  \
static Bool gsInitializeRegistered##classname =                              \
	classname::RegisterInitialize()

//----------------------------------------------------------------------------
#define WIRE_REGISTER_TERMINATE(classname)                                   \
	static Bool gsTerminateRegistered##classname =                           \
	classname::RegisterTerminate()

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_INITIALIZE(classname)                                 \
Bool classname::msInitializeRegistered = false;                              \
Bool classname::RegisterInitialize()                                         \
{                                                                            \
	if (!msInitializeRegistered)                                             \
	{                                                                        \
		Main::AddInitializer(classname::Initialize);                         \
		msInitializeRegistered = true;                                       \
	}                                                                        \
                                                                             \
	return msInitializeRegistered;                                           \
}

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_TERMINATE(classname)                                  \
Bool classname::msTerminateRegistered = false;                               \
Bool classname::RegisterTerminate()                                          \
{                                                                            \
	if (!msTerminateRegistered)                                              \
	{                                                                        \
		Main::AddTerminator(classname::Terminate);                           \
		msTerminateRegistered = true;                                        \
	}                                                                        \
	                                                                         \
	return msTerminateRegistered;                                            \
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
