// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include <sys/timeb.h>

//----------------------------------------------------------------------------
Double System::GetTime()
{
	static Long sInitialSec = 0;
	static Long sInitialUSec = 0;

	struct timeb tb;

	if (!msInitializedTime)
	{
		msInitializedTime = true;
		ftime(&tb);
		sInitialSec = static_cast<Long>(tb.time);
		sInitialUSec = 1000 * tb.millitm;
	}

	ftime(&tb);
	Long currentSec = static_cast<Long>(tb.time);
	Long currentUSec = 1000 * tb.millitm;
	Long deltaSec = currentSec - sInitialSec;
	Long deltaUSec = currentUSec - sInitialUSec;
	if (deltaUSec < 0)
	{
		deltaUSec += 1000000;
		deltaSec--;
	}

	return 0.001 * static_cast<Double>(1000*deltaSec + deltaUSec/1000);
}

//----------------------------------------------------------------------------
void System::Print(const Char* pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	vprintf(pFormat, args);

	va_end(args);
}

//----------------------------------------------------------------------------
void System::Assert(const Char* pExpression, const Char* pFile,
					Int lineNumber)
{
	Print("Assertion failed, %s, %s, line %d", pExpression, pFile,
		lineNumber);
}

//----------------------------------------------------------------------------
FILE* System::Fopen(const Char* pFilename, const Char* pMode)
{
	FILE* pFile;
	errno_t error = fopen_s(&pFile, pFilename, pMode);
	if (error == 0)
	{
		return pFile;
	}
	else
	{
		return NULL;
	}
}

//----------------------------------------------------------------------------
Char* System::Strcpy(Char* pDst, size_t dstSize, const Char* pSrc)
{
	errno_t error = strcpy_s(pDst, dstSize, pSrc);
	if (error == 0)
	{
		return pDst;
	}
	else
	{
		return NULL;
	}
}

//----------------------------------------------------------------------------
Char* System::Strcat(Char* pDst, size_t dstSize, const Char* pSrc)
{
	errno_t error = strcat_s(pDst, dstSize, pSrc);
	if (error == 0)
	{
		return pDst;
	}
	else
	{
		return NULL;
	}
}

//----------------------------------------------------------------------------
Char* System::Strncpy(Char* pDst, size_t dstSize, const Char* pSrc,
	size_t srcSize)
{
	errno_t error = strncpy_s(pDst, dstSize, pSrc, srcSize);
	if (error == 0)
	{
		return pDst;
	}
	else
	{
		return NULL;
	}
}
