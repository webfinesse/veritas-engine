#ifndef H_JOB
#define H_JOB

#include <atomic>
#include <functional>


namespace VeritasEngine
{
	struct Job;

	using JobFunction = std::function<void(Job*, const void*)>;

	constexpr unsigned JOB_DATA_SPACE{ 128 - sizeof(JobFunction) - sizeof(Job*) - sizeof(std::atomic_int32_t) };

	struct Job
	{
		JobFunction Callback; // 64 bytes on 64, 40 on 32 bit
		Job* Parent; // 8 bytes on 64, 4 on 32 bit
		std::atomic_int32_t UnfinishedJobs; // 4 bytes on 64
		char Data[JOB_DATA_SPACE];
	};

	
}

#endif
