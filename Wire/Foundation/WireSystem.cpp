#include "WireSystem.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

using namespace Wire;

#ifdef WIRE_WII
#include "Wii/WireSystemWii.inl"
#else
#include "Dx9/WireSystemDx9.inl"
#endif

Bool System::msInitializedTime = false;

//----------------------------------------------------------------------------
void* System::Memcpy(void* pDst, size_t dstSize, const void* pSrc,
	size_t srcSize)
{
	if (!pDst || dstSize == 0 || !pSrc || srcSize == 0)
	{
		// Be consistent with the behavior of memcpy_s.
		return NULL;
	}

	if (srcSize > dstSize)
	{
		// The source memory is too large to copy to the destination. To
		// be consistent with memcpy_s, return null as an indication that the
		// copy failed.
		return NULL;
	}

	memcpy(pDst, pSrc, srcSize);
	return pDst;
}

//----------------------------------------------------------------------------
void System::Memset(void* pDst, Int value, size_t count)
{
	memset(pDst, value, count);
}

//----------------------------------------------------------------------------
void System::Print(const Char* pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	vprintf(pFormat, args);

	va_end(args);
}
