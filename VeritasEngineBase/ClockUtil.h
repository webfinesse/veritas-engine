#ifndef H_CLOCKUTIL
#define H_CLOCKUTIL

#include <cstdint>

namespace VeritasEngine
{
	class ClockUtil
	{
	public:
		static uintmax_t ReadClock();
		static uintmax_t GetClockFrequency();
	};
}

#endif
