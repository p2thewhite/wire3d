#include "WireSystem.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

using namespace Wire;

#ifdef WIRE_WII
ULongLong System::msInitialTicks = 0;
#else
#include <sys/timeb.h>
Long System::msInitialUSec = 0;
Long System::msInitialSec = 0;
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
Double System::GetTime()
{
#ifdef WIRE_WII
	if (!msInitializedTime)
	{
		msInitializedTime = true;
		msInitialTicks = gettime();
	}

	ULongLong ticks = gettime();
	ULongLong deltaTicks = ticks - msInitialTicks;

	Double deltaSec = static_cast<Double>(ticks_to_millisecs(deltaTicks));

	return 0.001 * deltaSec;
#else
	struct timeb tb;

	if (!msInitializedTime)
	{
		msInitializedTime = true;
		ftime(&tb);
 		msInitialSec = static_cast<Long>(tb.time);
 		msInitialUSec = 1000 * tb.millitm;
	}

	ftime(&tb);
	Long currentSec = static_cast<Long>(tb.time);
	Long currentUSec = 1000 * tb.millitm;
	Long deltaSec = currentSec - msInitialSec;
	Long deltaUSec = currentUSec - msInitialUSec;
	if (deltaUSec < 0)
	{
		deltaUSec += 1000000;
		deltaSec--;
	}

	return 0.001 * static_cast<Double>(1000*deltaSec + deltaUSec/1000);
#endif
}

//----------------------------------------------------------------------------
void System::Print(const Char* pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	vprintf(pFormat, args);

	va_end(args);
}
