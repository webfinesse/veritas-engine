#ifndef H_JOBMANAGER
#define H_JOBMANAGER
#include <memory>

#include "SmallObject.h"
#include "IJobManager.h"

namespace VeritasEngine
{
	class ILogger;

	class JobManager : public SmallObject<>, public IJobManager
	{
	public:
		JobManager(std::shared_ptr<ILogger> logger);
		JobManager(JobManager&& other) noexcept;
		~JobManager() override = default;

		void Init() override;

		Job* CreateJob(JobFunction&& function) override;
		Job* CreateJobAsChild(Job* parent, JobFunction&& jobFunction) override;
		Job* CreateJobAsContinuation(Job* parent, JobFunction&& jobFunction) override;

		void Run(Job* job) override;
		void Wait(Job* job) override;
		

		JobManager& operator=(JobManager&& other) noexcept;

	private:
		struct Impl;
		struct WorkQueue;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif

