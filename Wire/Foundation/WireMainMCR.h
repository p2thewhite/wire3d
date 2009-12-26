#pragma once
#ifndef WIREMAINMCR_H
#define WIREMAINMCR_H

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
}

#endif
