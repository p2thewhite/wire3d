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
		#define WIRE_ASSERT(expression) if (!(expression)) System::Assert(#expression, __FILE__, __LINE__);
//		#define WIRE_ASSERT(expression)
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
#include <limits.h>
#include <stdio.h>

#include "WireTypes.h"
#include "WireMemory.h"

namespace Wire
{

class System
{
public:
	static UInt MaxUInt;
	static Int MaxInt;

	static void* Memcpy(void* pDst, size_t dstSize, const void* pSrc,
		size_t srcSize);
	static void Memset(void* pDst, Int value, size_t count);
	static Int Memcmp(const void* pBuf1, const void* pBuf2, size_t size);

	static Double GetTime();
	static void Print(const Char* pFormat, ...);
	static void Assert(const Char* pExpression, const Char* pFile,
		Int lineNumber);

	// File support for buffer load and save.
	static Bool Load(const Char* pFilename, Char*& rBuffer, Int& rSize);
	static Bool Load(const Char* pFilename, UChar*& rBuffer, Int& rSize);
	static Bool Save(const Char* pFilename, const Char* pBuffer, Int size);
	static Bool Save(const Char* pFilename, const UChar* pBuffer, Int size);

	static FILE* Fopen(const Char* pFilename, const Char* pMode);
 	static Int Fprintf(FILE* pFile, const Char* pFormat, ...);
	static Int Fclose(FILE* pFile);

	static Char* Strcpy(Char* pDst, size_t dstSize, const Char* pSrc);
	static Char* Strcat(Char* pDst, size_t dstSize, const Char* pSrc);
	static Char* Strncpy(Char* pDst, size_t dstSize, const Char* pSrc,
        size_t srcSize);
	static UInt Strlen(const Char* pString);

	static Int Toupper(Int c);
	static Int Tolower(Int c);

private:
	static Bool msInitializedTime;
};

}

#endif
