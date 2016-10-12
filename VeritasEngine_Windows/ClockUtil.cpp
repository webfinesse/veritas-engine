#include "../VeritasEngineBase/ClockUtil.h"

#include <Windows.h>

uintmax_t VeritasEngine::ClockUtil::GetClockFrequency()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	return freq.QuadPart;
}

uintmax_t VeritasEngine::ClockUtil::ReadClock()
{
	LARGE_INTEGER largeInteger;
	QueryPerformanceCounter(&largeInteger);

	return largeInteger.QuadPart;
}
