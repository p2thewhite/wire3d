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
