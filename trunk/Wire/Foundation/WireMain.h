#pragma once
#ifndef WIREMAIN_H
#define WIREMAIN_H

#include "WireTArray.h"

namespace Wire
{

// TODO: move Main to Engine
class /*WIRE_FOUNDATION_ITEM*/ Main
{

public:
	typedef void (*Initializer)();
	typedef TArray<Initializer> InitializerArray;
	static void AddInitializer(Initializer initializer);

	static void Initialize();

private:
	static InitializerArray* smpInitializers;

};

}

#endif
