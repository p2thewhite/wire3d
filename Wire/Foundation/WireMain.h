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

private:
	static InitializerArray* smpInitializers;

};

}

#endif
