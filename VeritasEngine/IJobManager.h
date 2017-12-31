#ifndef H_IJOBMANAGER
#define H_IJOBMANAGER

#include "Job.h"
#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	class EXPORT IJobManager
	{
	public:
		virtual ~IJobManager() = default;
		virtual void Init() = 0;

		virtual Job* CreateJob(JobFunction&& function) = 0;
		virtual Job* CreateJobAsChild(Job* parent, JobFunction&& jobFunction) = 0;
		virtual Job* CreateJobAsContinuation(Job* parent, JobFunction&& jobFunction) = 0;

		virtual void Run(Job* job) = 0;
		virtual void Wait(Job* job) = 0;
	};
}

#endif
