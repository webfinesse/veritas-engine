#include "JobManager.h"

#include <vector>
#include <mutex>

#include <thread>
#include <algorithm>
#include <cassert>

#include "Job.h"
#include "ILogger.h"
#include "../VeritasEngineBase/StringHelper.h"

#define TRACE_ENABLED

#ifdef TRACE_ENABLED
#define TRACE(...) Trace(__VA_ARGS__);
#define TRACEINTERNAL(...) logger->Trace(__VA_ARGS__);
#else
#define TRACE(...) 
#define TRACEINTERNAL(...) 
#endif

// based on the job system detailed at molecular musings: https://blog.molecular-matters.com/2015/08/24/job-system-2-0-lock-free-work-stealing-part-1-basics/

constexpr unsigned int NUMBER_OF_JOBS{ 4096 };
constexpr unsigned int NUMBER_OF_JOBS_MASK{ 4096 - 1 };
constexpr unsigned int MAX_NUMBER_OF_QUEUES{ 8 };
constexpr char JOBMANAGER_LOGGING_CATEGORY[] = "Job Manager";

bool g_runJobs { true };
std::atomic_int32_t g_activeJobCount{ 0 };
std::atomic_int32_t g_jobId{ 0 };
std::condition_variable g_hasJobsConditionVariable{};

struct VeritasEngine::JobManager::WorkQueue
{
	WorkQueue()
	{
		std::memset(&m_jobs, 0, sizeof(m_jobs));
	}

	void Push(Job* job, unsigned queueIndex, ILogger* logger)
	{
		const auto b = m_bottom;
		m_jobs[b & NUMBER_OF_JOBS_MASK] = job;

		std::atomic_signal_fence(std::memory_order_acquire);

		TRACEINTERNAL(JOBMANAGER_LOGGING_CATEGORY, "Pushing Job %d on queue %d at index %d", job->Id, queueIndex, b)

		m_bottom = b + 1;
	}

	Job* Pop(ILogger* logger, unsigned queueIndex)
	{
		const auto b = m_bottom - 1;
		_InterlockedExchange(&m_bottom, b);

		auto t = m_top;
		if(t <= b)
		{
			auto job = m_jobs[b & NUMBER_OF_JOBS_MASK];

			if(t != b)
			{
				TRACEINTERNAL(JOBMANAGER_LOGGING_CATEGORY, "Popping job from queue %d at bottom index %d", queueIndex, b)
				return job;
			}

			if(_InterlockedCompareExchange(&m_top, t + 1, t) != t)
			{
				TRACEINTERNAL(JOBMANAGER_LOGGING_CATEGORY, "Lost Pop CompareExchange from queue %d at top index %d", queueIndex, t)
				job = nullptr;
			}

			TRACEINTERNAL(JOBMANAGER_LOGGING_CATEGORY, "Won Pop CompareExchange from queue %d at top index %d, returning index %d", queueIndex, t, b)

			m_bottom = t + 1;
			return job;
		}
		else
		{
			m_bottom = t;
			return nullptr;
		}
	}

	Job* Steal(ILogger* logger, unsigned queueIndex)
	{
		auto t = m_top;

		std::atomic_signal_fence(std::memory_order_acquire);

		const auto b = m_bottom;
		
		if(t < b)
		{
			const auto job = m_jobs[t & NUMBER_OF_JOBS_MASK];

			
			if(_InterlockedCompareExchange(&m_top, t+1, t) != t)
			{
				TRACEINTERNAL(JOBMANAGER_LOGGING_CATEGORY, "Lost Steal CompareExchange from queue %d at top index %d", queueIndex, t)
				return nullptr;
			}

			TRACEINTERNAL(JOBMANAGER_LOGGING_CATEGORY, "Won Pop CompareExchange from queue %d at top index %d, returning index %d", queueIndex, t, t)

			return job;
		}
		else
		{
			return nullptr;
		}
	}

private:
	Job* m_jobs[NUMBER_OF_JOBS];
	volatile long m_bottom{ 0 };
	volatile long m_top{ 0 };
};

struct VeritasEngine::JobManager::Impl
{
	Impl(std::shared_ptr<ILogger>&& logger)
	: m_queueCount{ std::min(std::thread::hardware_concurrency(), MAX_NUMBER_OF_QUEUES) }, m_logger{ std::move(logger) }
	{
		static_assert(sizeof(Job) == JOB_ALIGNMENT_SIZE);
		static_assert(std::atomic_int32_t::is_always_lock_free && "Platform is not lock free");

#ifdef TRACE_ENABLED
		m_logger->SetIsEnabled(true);
#endif
	}

	void Init()
	{
		m_queueIndex = 0;

		m_workerThreads.reserve(m_queueCount - 1);

		TRACE("Initializing Job System with %u threads", m_queueCount)

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
		if (job && job->UnfinishedJobs.load() > 0)
		{
			g_activeJobCount.fetch_add(1);

			TRACE("Pushing Job %u on thread %u. There are %d active jobs", job->Id, m_queueIndex, g_activeJobCount.load())

			GetQueue().Push(job, m_queueIndex, m_logger.get());
			g_hasJobsConditionVariable.notify_all();
		}
	}

	static Job* AllocateJob()
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
		TRACE("Executing Job %u on thread %u", job->Id, m_queueIndex)
		(job->Callback)(job);
		FinishJob(job);
	}

	Job* GetJob()
	{
		auto& queue = GetQueue();

		const auto job = queue.Pop(m_logger.get(), m_queueIndex);
		if (job == nullptr)
		{
			auto& otherQueue = m_queues[0];

			if (&queue == &otherQueue)
			{
				std::this_thread::yield();
				return nullptr;
			}

			Job* stolenJob = otherQueue.Steal(m_logger.get(), 0);

			if (stolenJob)
			{
				TRACE("Stole Job %u from thread 0 with thread %u", stolenJob->Id, m_queueIndex)
				return stolenJob;
			}

			const auto queueIndex = rand() % m_queueCount;
			auto& randomQueue = m_queues[queueIndex];

			if (&queue == &randomQueue || &otherQueue == &randomQueue)
			{
				std::this_thread::yield();
				return nullptr;
			}

			stolenJob = randomQueue.Steal(m_logger.get(), queueIndex);

			if (stolenJob == nullptr)
			{
				std::this_thread::yield();
				return nullptr;
			}

			TRACE("Stole Job %u from thread %u with thread %u", stolenJob->Id, queueIndex, m_queueIndex)

			return stolenJob;
		}
		return job;
	}

private:
	void FinishJob(Job* job)
	{
		assert(job->UnfinishedJobs.load() > 0);

		const auto jobCount = job->UnfinishedJobs.fetch_sub(1) - 1;

		if (jobCount == 0)
		{
			g_activeJobCount.fetch_sub(1);

			TRACE("Finishing Job %u with thread %u, there are %d active jobs remaining", job->Id, m_queueIndex, g_activeJobCount.load())

			if (job->Parent)
			{
				TRACE("Finishing Job %u (Parent Id: %u) with thread %u, there are %u unfinished parent jobs", job->Id, job->Parent->Id, m_queueIndex, job->Parent->UnfinishedJobs.load())
				FinishJob(job->Parent);
			}
		}
		else
		{
			TRACE("Finishing root Job %u with thread %u, there are %u unfinished children", job->Id, m_queueIndex, jobCount)
		}

		const auto continuationJobCount = job->ContinousJobCount.load();
		if (continuationJobCount > 0)
		{
			for(int i = 0; i < continuationJobCount; i++)
			{
				Push(job->Continuations[i]);
			}
		}

		assert(g_activeJobCount.load() >= 0);
	}

#ifdef TRACE_ENABLED
	template <typename ...Args>
	void Trace(const char* message, Args... args)
	{
		m_logger->Trace(JOBMANAGER_LOGGING_CATEGORY, message, args...);
	}
#endif

	static void ThreadMain(Impl* impl, const unsigned int queueIndex)
	{
		m_queueIndex = queueIndex;

		std::unique_lock<std::mutex> lock{ m_threadMainMutex };
				
		while (g_runJobs)
		{
			g_hasJobsConditionVariable.wait(lock, []{ return g_activeJobCount.load() > 0 || !g_runJobs; });

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
	std::shared_ptr<ILogger> m_logger;
	WorkQueue m_queues[MAX_NUMBER_OF_QUEUES];
	std::vector<std::thread> m_workerThreads{};
};

thread_local unsigned int VeritasEngine::JobManager::Impl::m_queueIndex;
thread_local unsigned int VeritasEngine::JobManager::Impl::m_allocatedJobs { 0 };
thread_local std::mutex VeritasEngine::JobManager::Impl::m_threadMainMutex{};
thread_local VeritasEngine::Job VeritasEngine::JobManager::Impl::m_jobs[NUMBER_OF_JOBS];

VeritasEngine::JobManager::JobManager(std::shared_ptr<ILogger> logger)
	: m_impl{std::make_unique<Impl>(std::move(logger)) }
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
	auto job = m_impl->AllocateJob();
	job->Callback = std::move(function);
	job->Parent = nullptr;
	job->UnfinishedJobs = 1;
	job->ContinousJobCount = 0;
	job->Id = ++g_jobId;

	return job;
}



VeritasEngine::Job* VeritasEngine::JobManager::CreateJobAsChild(Job* parent, JobFunction&& jobFunction)
{
	parent->UnfinishedJobs.fetch_add(1);

	auto job = m_impl->AllocateJob();
	job->Callback = std::move(jobFunction);
	job->Parent = parent;
	job->UnfinishedJobs = 1;
	job->Id = ++g_jobId;

	return job;
}

VeritasEngine::Job* VeritasEngine::JobManager::CreateJobAsContinuation(Job* parent, JobFunction&& jobFunction)
{
	assert(parent->ContinousJobCount < NUMBER_JOB_CONTINUATIONS);
	
	if (parent->UnfinishedJobs > 0)
	{
		const auto index = parent->ContinousJobCount++;

		const auto job = CreateJob(std::move(jobFunction));

		parent->Continuations[index] = job;

		return job;
	}

	return nullptr;
}


void VeritasEngine::JobManager::Run(Job* job)
{
	m_impl->Push(job);
}

void VeritasEngine::JobManager::Wait(Job* job)
{
	while(job && job->UnfinishedJobs.load() > 0)
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
