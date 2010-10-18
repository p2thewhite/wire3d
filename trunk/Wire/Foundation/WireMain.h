#pragma once
#ifndef WIREMAIN_H
#define WIREMAIN_H

#include "WireTArray.h"
#include "WireMainMCR.h"

namespace Wire
{

class /*WIRE_FOUNDATION_ITEM*/ Main
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

#endif
