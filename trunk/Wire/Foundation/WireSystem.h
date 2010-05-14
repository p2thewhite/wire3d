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
	#ifdef WIRE_DEBUG
		#define WIRE_ASSERT(expression) if (!(expression)) System::Assert(#expression, __FILE__, __LINE__);
	#else
		#define WIRE_ASSERT(expression)
	#endif
#else
	#include <stddef.h>
	#include <assert.h>
	#include <cmath>
	#ifdef WIRE_DEBUG
		#define WIRE_ASSERT(expression) assert(expression)
	#else
		#define WIRE_ASSERT(expression)
	#endif
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
	static void Memset(void* pDst, Int value, size_t count);

	static Double GetTime();
	static void Print(const Char* pFormat, ...);
	static void Assert(const Char* pExpression, const Char* pFile,
		Int lineNumber);

private:
	static Bool msInitializedTime;
};

}

#endif
