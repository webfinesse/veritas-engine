#include "JobManager.h"

#include <vector>
#include <mutex>

#include <thread>
#include <algorithm>
#include <cassert>

#include "Job.h"

#if _WIN32 || _WIN64
#include <immintrin.h>
#define YIELD_COMMAND _mm_pause();
#else
static_assert(0 && "YIELD COMMAND Not Specified For Platform")
#endif



// based on the job system detailed at molecular musings: https://blog.molecular-matters.com/2015/08/24/job-system-2-0-lock-free-work-stealing-part-1-basics/

constexpr unsigned int NUMBER_OF_JOBS{ 4096 };
constexpr unsigned int NUMBER_OF_JOBS_MASK{ 4096 - 1 };
constexpr unsigned int MAX_NUMBER_OF_QUEUES{ 8 };

bool g_runJobs { true };
std::condition_variable g_hasJobsConditionVariable{};

struct VeritasEngine::JobManager::WorkQueue
{
	void Push(Job* job)
	{
		const auto b = m_bottom.fetch_add(1);
		m_jobs[b & NUMBER_OF_JOBS_MASK] = job;		
	}

	Job* Pop()
	{
		--m_bottom;
		const auto b = m_bottom.load();
		auto t = m_top.load();
		if(t <= b)
		{
			auto job = m_jobs[b & NUMBER_OF_JOBS_MASK];

			if(t != b)
			{
				return job;
			}

			if(!m_top.compare_exchange_strong(t, t+1))
			{
				job = nullptr;
			}

			m_bottom = t + 1;
			return job;
		}
		else
		{
			m_bottom = t;
			return nullptr;
		}
	}

	Job* Steal()
	{
		auto t = m_top.load();
		const auto b = m_bottom.load();
		if(t < b)
		{
			const auto job = m_jobs[t & NUMBER_OF_JOBS_MASK];

			if(!m_top.compare_exchange_strong(t, t+1))
			{
				return nullptr;
			}

			return job;
		}
		else
		{
			return nullptr;
		}
	}

private:
	Job* m_jobs[NUMBER_OF_JOBS];
	std::atomic_long m_bottom{ 0 };
	std::atomic_long m_top{ 0 };
	
};

struct VeritasEngine::JobManager::Impl
{
	Impl()
	: m_queueCount{ std::min(std::thread::hardware_concurrency(), MAX_NUMBER_OF_QUEUES) }
	{
		static_assert(std::atomic_int32_t::is_always_lock_free == true && "Platform is not lock free");
	}

	void Init()
	{
		m_queueIndex = 0;

		m_workerThreads.reserve(m_queueCount - 1);

		for (unsigned i = 1; i < m_queueCount; i++)
		{
			m_workerThreads.emplace_back(ThreadMain, this, i);
		}
	}

	~Impl()
	{
		g_runJobs = false;
		g_hasJobsConditionVariable.notify_all();

		for(auto& t : m_workerThreads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		m_workerThreads.clear();
	}

	void Push(Job* job)
	{
		if (job && job->UnfinishedJobs > 0)
		{
			GetQueue().Push(job);
			++m_activeJobCount;
			g_hasJobsConditionVariable.notify_all();
		}
	}

	Job* AllocateJob()
	{
		const auto index = m_allocatedJobs++;
		return &m_jobs[index & NUMBER_OF_JOBS_MASK];
	}

	WorkQueue& GetQueue()
	{
		return m_queues[m_queueIndex];
	}

	void ExecuteJob(Job* job)
	{
		(job->Callback)(job, job->Data);
		FinishJob(job);
	}

	Job* GetJob()
	{
		auto& queue = GetQueue();

		const auto job = queue.Pop();
		if (job == nullptr)
		{
			auto& otherQueue = m_queues[0];

			if (&queue == &otherQueue)
			{
				Yield();
				return nullptr;
			}

			Job* stolenJob = otherQueue.Steal();

			if (stolenJob)
			{
				return stolenJob;
			}

			auto& randomQueue = m_queues[rand() % m_queueCount];

			if (&queue == &randomQueue)
			{
				Yield();
				return nullptr;
			}

			stolenJob = randomQueue.Steal();

			if (stolenJob == nullptr)
			{
				Yield();
				return nullptr;
			}

			return stolenJob;
		}
		return job;
	}

private:
	void FinishJob(Job* job)
	{
		const auto jobCount = --job->UnfinishedJobs;

		if (jobCount == 0 && (job->Parent))
		{
			FinishJob(job->Parent);
		}

		--m_activeJobCount;
	}

	static void Yield()
	{
		YIELD_COMMAND
	}

	static void ThreadMain(Impl* impl, const unsigned int queueIndex)
	{
		m_queueIndex = queueIndex;

		std::unique_lock<std::mutex> lock{ m_threadMainMutex };
				
		while (g_runJobs)
		{
			g_hasJobsConditionVariable.wait(lock, [&] { return impl->m_activeJobCount > 0 || !g_runJobs; });

			// double check if we need to run jobs since we may be trying to shut down the engine
			if (g_runJobs)
			{
				const auto job = impl->GetJob();
				if (job != nullptr)
				{
					impl->ExecuteJob(job);
				}
			}
		}		
	}

	static thread_local unsigned int m_queueIndex;
	static thread_local unsigned int m_allocatedJobs;
	static thread_local std::mutex m_threadMainMutex;
	static thread_local Job m_jobs[NUMBER_OF_JOBS];

	unsigned int m_queueCount;
	std::atomic_int32_t m_activeJobCount{ 0 };
	WorkQueue m_queues[MAX_NUMBER_OF_QUEUES];
	std::vector<std::thread> m_workerThreads{};
};

thread_local unsigned int VeritasEngine::JobManager::Impl::m_queueIndex;
thread_local unsigned int VeritasEngine::JobManager::Impl::m_allocatedJobs { 0 };
thread_local std::mutex VeritasEngine::JobManager::Impl::m_threadMainMutex{};
thread_local VeritasEngine::Job VeritasEngine::JobManager::Impl::m_jobs[NUMBER_OF_JOBS];

VeritasEngine::JobManager::JobManager()
	: m_impl{std::make_unique<Impl>() }
{

}

VeritasEngine::JobManager::JobManager(JobManager&& other) noexcept
	: m_impl{ std::move(other.m_impl) }
{

}

void VeritasEngine::JobManager::Init()
{
	m_impl->Init();
}

VeritasEngine::Job* VeritasEngine::JobManager::CreateJob(JobFunction&& function)
{
	return CreateJob(std::move(function), nullptr, 0);
}

VeritasEngine::Job* VeritasEngine::JobManager::CreateJob(JobFunction&& function, void* data, const size_t sizeOfData)
{
	assert(sizeOfData < sizeof(Job::Data));

	auto job = m_impl->AllocateJob();
	job->Callback = std::move(function);
	job->Parent = nullptr;
	job->UnfinishedJobs = 1;

	if (data)
	{
		std::memcpy(job->Data, data, sizeOfData);
	}

	return job;
}

VeritasEngine::Job* VeritasEngine::JobManager::CreateJobFromResult(void* data, const size_t sizeOfData)
{
	assert(sizeOfData < sizeof(Job::Data));

	auto job = m_impl->AllocateJob();
	job->Callback = nullptr;
	job->Parent = nullptr;
	job->UnfinishedJobs = 0;

	if (data)
	{
		std::memcpy(job->Data, data, sizeOfData);
	}

	return job;
}

void VeritasEngine::JobManager::SetJobResult(Job* job, void* data, const size_t sizeOfData)
{
	assert(sizeOfData < sizeof(Job::Data));

	std::memcpy(job->Data, data, sizeOfData);
}

void* VeritasEngine::JobManager::GetJobResult(Job* job)
{
	return job->Data;
}

VeritasEngine::Job* VeritasEngine::JobManager::CreateJobAsChild(Job* parent, JobFunction&& jobFunction)
{
	return CreateJobAsChild(parent, std::move(jobFunction), nullptr, 0);
}

VeritasEngine::Job* VeritasEngine::JobManager::CreateJobAsChild(Job* parent, JobFunction&& jobFunction, void* data, const size_t sizeOfData)
{
	++parent->UnfinishedJobs;

	auto job = m_impl->AllocateJob();
	job->Callback = std::move(jobFunction);
	job->Parent = parent;
	job->UnfinishedJobs = 1;

	if (data)
	{
		std::memcpy(job->Data, data, sizeOfData);
	}

	return job;
}

void VeritasEngine::JobManager::Run(Job* job)
{
	m_impl->Push(job);
}

void VeritasEngine::JobManager::Wait(Job* job)
{
	while(job && job->UnfinishedJobs > 0)
	{
		const auto nextJob = m_impl->GetJob();

		if(nextJob != nullptr)
		{
			m_impl->ExecuteJob(nextJob);
		}
	}
}

void VeritasEngine::JobManager::WaitAll(std::initializer_list<Job*> jobs)
{
	for(const auto& job : jobs)
	{
		Wait(job);
	}
}

VeritasEngine::JobManager& VeritasEngine::JobManager::operator=(JobManager&& other) noexcept
{
	if(this != &other)
	{
		this->m_impl = std::move(other.m_impl);
	}

	return *this;
}
