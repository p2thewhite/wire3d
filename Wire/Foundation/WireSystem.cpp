#include "WireSystem.h"
#include <string.h>

using namespace Wire;

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
