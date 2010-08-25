#include "WireSystem.h"
#include <cctype>
#include <string.h>
#include <stdarg.h>

using namespace Wire;

#ifdef WIRE_WII
#include "Wii/WireSystemWii.inl"
#else
#include "Dx9/WireSystemDx9.inl"
#endif

Bool System::msInitializedTime = false;
UInt System::MaxUInt = UINT_MAX;
Int System::MaxInt = INT_MAX;

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
Int System::Memcmp(const void* pBuf1, const void* pBuf2, size_t size)
{
	return memcmp(pBuf1, pBuf2, size);
}

//----------------------------------------------------------------------------
Int System::Fclose(FILE* pFile)
{
	fflush(pFile);
	return fclose(pFile);
}

//----------------------------------------------------------------------------
Int System::Fprintf(FILE* pFile, const Char* pFormat, ...)
{
	if (!pFile || !pFormat)
	{
		return -1;
	}

	va_list args;
	va_start(args, pFormat);

	Int numWritten = vfprintf(pFile, pFormat, args);

	va_end(args);
	return numWritten;
}

//----------------------------------------------------------------------------
UInt System::Strlen(const Char* pString)
{
	return static_cast<UInt>(strlen(pString));
}

//----------------------------------------------------------------------------
Int System::Toupper(Int c)
{
	return toupper(c);
}

//----------------------------------------------------------------------------
Int System::Tolower(Int c)
{
	return tolower(c);
}
