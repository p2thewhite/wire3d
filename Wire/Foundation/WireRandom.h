#pragma once
#ifndef WIRERANDOM_H
#define WIRERANDOM_H

#include "WireTypes.h"

namespace Wire
{
// based on "Mother-of-all generator" from http://www.agner.org/random/

class Random
{
public:
	Random(UInt seed = 0);

	void Init(UInt seed);		 // initialization

	// Get random number.
	UInt Get();

	// Get random number, interval [0,1]
	Float GetFloat();
	Double GetDouble();

protected:
	ULongLong mX[5];
};

}

#endif
