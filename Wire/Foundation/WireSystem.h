#pragma once
#ifndef WIRESYSTEM_H
#define WIRESYSTEM_H

#ifdef WIRE_WIN
#ifdef WIRE_WII
#undef WIRE_WII
#endif /* WIRE_WII */
#else
#define WIRE_WII
#endif /* WIRE_WIN */

#ifdef WIRE_WII
#include <math.h>
#include "Wii/WireOgcDefines.h"
#define WIRE_ASSERT(condition)
#else
#include <stddef.h>
#include <assert.h>
#include <cmath>
#define WIRE_ASSERT(condition) assert(condition)
#endif /* WIRE_WII */

#include <float.h>

#include "WireTypes.h"
#include "WireMemory.h"

namespace Wire
{

class /*WIRE_FOUNDATION_ITEM*/ System
{
public:
	static void* Memcpy(void* pDst, size_t dstSize, const void* pSrc,
		size_t srcSize);
	static void Memset(void* pDst, int value, size_t count);

	static Double GetTime();
	static void Print(const Char* pFormat, ...);

private:
	static Bool msInitializedTime;
#ifdef WIRE_WII
	static ULongLong msInitialTicks;
#else
	static Long msInitialSec;
	static Long msInitialUSec;
#endif
};

}

#endif
