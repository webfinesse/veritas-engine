#ifndef H_JOB
#define H_JOB

#include <atomic>
#include <functional>


namespace VeritasEngine
{
	struct Job;

	using JobFunction = std::function<void(Job*, const void*)>;

#if _WIN32 || _WIN64
#if _WIN64
	constexpr unsigned JOB_DATA_SPACE{ 52 };
#else
	constexpr unsigned JOB_DATA_SPACE{ 80 };
#endif
#else
	static_assert(0 && "Padding For Job Not Specified For Platform")
#endif

	struct Job
	{
		JobFunction Callback; // 64 bytes on 64, 40 on 32 bit
		Job* Parent; // 8 bytes on 64, 4 on 32 bit
		std::atomic_int32_t UnfinishedJobs; // 4 bytes on 64
		char Data[JOB_DATA_SPACE];
	};

	
}

#endif
