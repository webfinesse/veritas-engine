#ifndef H_JOB
#define H_JOB

#include <atomic>
#include <functional>


namespace VeritasEngine
{
	struct Job;

	using JobFunction = std::function<void(Job*)>;

	constexpr unsigned JOB_ALIGNMENT_SIZE{ 128 };
	constexpr int32_t NUMBER_JOB_CONTINUATIONS{ 5 };

	constexpr unsigned JOB_DATA_SPACE{ JOB_ALIGNMENT_SIZE - sizeof(JobFunction) - sizeof(Job*) - (sizeof(std::atomic_int32_t) * 2) - (sizeof(Job*) * NUMBER_JOB_CONTINUATIONS) - sizeof(unsigned) };

	struct Job
	{
		JobFunction Callback; // 64 bytes on 64, 40 on 32 bit
		Job* Parent; // 8 bytes on 64, 4 on 32 bit
		std::atomic_int32_t UnfinishedJobs; // 4 bytes on 64
		std::atomic_int32_t ContinousJobCount; // 4 bytes on 64
		unsigned Id;
		Job* Continuations[NUMBER_JOB_CONTINUATIONS]; // 48 bytes on 64
	};

	
}

#endif
