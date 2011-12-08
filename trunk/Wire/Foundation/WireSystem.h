// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

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
#else
	#include <stddef.h>
	#include <assert.h>
	#include <cmath>
#endif /* WIRE_WII */

#ifdef WIRE_DEBUG
	#define WIRE_ASSERT(e) \
	(void)((!!(e)) || (System::Assert(#e, __FILE__, __LINE__), 0))
#else
	#define WIRE_ASSERT(expression)
#endif

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
	enum Platform
	{
		PF_WII,
		PF_DX9,
		PF_QUANTITY
	};

	static const UInt MAX_UINT;
	static const Int MAX_INT;

	static Bool IsBigEndian();

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
	static Int Strcmp(const Char* pStr1, const Char* pStr2);
	static Int Strncmp(const Char* pStr1, const Char* pStr2, size_t size);
	static Int Sprintf(Char* pDst, size_t dstSize, const Char* pFormat, ...);

	static Int Toupper(Int c);
	static Int Tolower(Int c);

	static Platform GetPlatform();

private:
	static Bool s_InitializedTime;
};

}

#endif
