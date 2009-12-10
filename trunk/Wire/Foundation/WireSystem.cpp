#include "WireSystem.h"
#include <string.h>

using namespace Wire;

#ifdef WIRE_WIN
#include <sys/timeb.h>
Long System::msInitialUSec = 0;
#endif

Long System::msInitialSec = 0;
Bool System::msInitializedTime = false;

//----------------------------------------------------------------------------
void* System::Memcpy(void* pDst, size_t dstSize, const void* pSrc,
	size_t srcSize)
{
	if (!pDst || dstSize == 0 || !pSrc || srcSize == 0)
	{
		// Be consistent with the behavior of memcpy_s.
		return 0;
	}

	if (srcSize > dstSize)
	{
		// The source memory is too large to copy to the destination.  To
		// be consistent with memcpy_s, return null as an indication that the
		// copy failed.
		return 0;
	}

	memcpy(pDst,pSrc,srcSize);
	return pDst;
}

//----------------------------------------------------------------------------
Double System::GetTime()
{
#ifdef WIRE_WII
	if (!msInitializedTime)
	{
		msInitializedTime = true;
		msInitialSec = gettime();
	}

	Long ticks = gettime();
	Long deltaTicks = ticks - msInitialSec;
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
	long currentSec = static_cast<Long>(tb.time);
	long currentUSec = 1000 * tb.millitm;
	long deltaSec = currentSec - msInitialSec;
	long deltaUSec = currentUSec - msInitialUSec;
	if (deltaUSec < 0)
	{
		deltaUSec += 1000000;
		deltaSec--;
	}

	return 0.001 * static_cast<Double>(1000*deltaSec + deltaUSec/1000);
#endif
}
