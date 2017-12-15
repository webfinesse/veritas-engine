#include "JobManager.h"

#include <vector>
#include <mutex>
#include <shared_mutex>
#include <immintrin.h>
#include <thread>
#include <algorithm>
#include <cassert>

#include "Job.h"



// based on the job system detailed at molecular musings: https://blog.molecular-matters.com/2015/08/24/job-system-2-0-lock-free-work-stealing-part-1-basics/

constexpr unsigned int NUMBER_OF_JOBS{ 4096 };
constexpr unsigned int NUMBER_OF_JOBS_MASK{ 4096 - 1 };
constexpr unsigned int MAX_NUMBER_OF_QUEUES{ 8 };

bool g_runJobs { true };
std::condition_variable g_hasJobsConditionVariable{};

struct VeritasEngine::JobManager::WorkQueue : SmallObject<>
{
	void Push(Job* job)
	{
		std::scoped_lock<std::mutex> lock{ m_section };

		m_jobs[m_bottom & NUMBER_OF_JOBS_MASK] = job;
		++m_bottom;
	}

	Job* Pop()
	{
		std::scoped_lock<std::mutex> lock{ m_section };

		const auto jobCount = m_bottom - m_top;

		if (jobCount <= 0)
		{
			return nullptr;
		}

		--m_bottom;
		return m_jobs[m_bottom & NUMBER_OF_JOBS_MASK];
	}

	Job* Steal()
	{
		std::scoped_lock<std::mutex> lock{ m_section };

		const auto jobCount = m_bottom - m_top;
		

		if (jobCount <= 0)
		{
			return nullptr;
		}

		Job* job = m_jobs[m_top & NUMBER_OF_JOBS_MASK];
		++m_top;

		return job;
	}

private:
	Job* m_jobs[NUMBER_OF_JOBS];
	int m_bottom{ 0 };
	int m_top{ 0 };
	std::mutex m_section{};
};

struct VeritasEngine::JobManager::Impl : SmallObject<>
{
	Impl()
	: m_queueCount{ std::min(std::thread::hardware_concurrency(), MAX_NUMBER_OF_QUEUES) }
	{

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
		GetQueue().Push(job);
		++m_activeJobCount;
		g_hasJobsConditionVariable.notify_all();
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
		_mm_pause();
	}

	static void ThreadMain(Impl* impl, const unsigned int queueIndex)
	{
		m_queueIndex = queueIndex;

		std::unique_lock<std::mutex> lock{ m_threadMainMutex };
				
		while (g_runJobs)
		{
			g_hasJobsConditionVariable.wait(lock, [&] { return impl->m_activeJobCount > 0 || !g_runJobs; });

			const auto job = impl->GetJob();
			if (job != nullptr)
			{
				impl->ExecuteJob(job);
			}
		}		
	}

	static thread_local unsigned int m_queueIndex;
	static thread_local unsigned int m_allocatedJobs;
	static thread_local std::mutex m_threadMainMutex;
	static thread_local Job m_jobs[NUMBER_OF_JOBS];

	unsigned int m_queueCount; // 4
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
	while(job->UnfinishedJobs > 0)
	{
		const auto nextJob = m_impl->GetJob();

		if(nextJob != nullptr)
		{
			m_impl->ExecuteJob(nextJob);
		}
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
