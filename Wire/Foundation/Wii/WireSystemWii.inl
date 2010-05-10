//----------------------------------------------------------------------------
Double System::GetTime()
{
	static ULongLong msInitialTicks = 0;

	if (!msInitializedTime)
	{
		msInitializedTime = true;
		msInitialTicks = gettime();
	}

	ULongLong ticks = gettime();
	ULongLong deltaTicks = ticks - msInitialTicks;

	Double deltaSec = static_cast<Double>(ticks_to_millisecs(deltaTicks));

	return 0.001 * deltaSec;
}
