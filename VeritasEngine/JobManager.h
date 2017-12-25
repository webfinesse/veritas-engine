#ifndef H_JOBMANAGER
#define H_JOBMANAGER
#include <memory>

#include "SmallObject.h"
#include "IJobManager.h"

namespace VeritasEngine
{
	class JobManager : public SmallObject<>, public IJobManager
	{
	public:
		JobManager();
		JobManager(JobManager&& other) noexcept;
		~JobManager() override = default;

		void Init() override;

		Job* CreateJob(JobFunction&& function) override;
		Job* CreateJob(JobFunction&& function, void* data, const size_t sizeOfData) override;
		Job* CreateJobFromResult(void* data, const size_t sizeOfData) override;
		void SetJobResult(Job* job, void* data, const size_t sizeOfData) override;
		void* GetJobResult(Job* job) override;
		Job* CreateJobAsChild(Job* parent, JobFunction&& jobFunction) override;
		Job* CreateJobAsChild(Job* parent, JobFunction&& jobFunction, void* data, const size_t sizeOfData) override;

		void Run(Job* job) override;
		void Wait(Job* job) override;
		void WaitAll(std::initializer_list<Job*> jobs) override;

		JobManager& operator=(JobManager&& other) noexcept;

	private:
		struct Impl;
		struct WorkQueue;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif

