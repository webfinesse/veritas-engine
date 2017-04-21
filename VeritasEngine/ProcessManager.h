#ifndef H_PROCESSMANAGER
#define H_PROCESSMANAGER

#include <memory>
#include "Process.h"
#include "SmallObject.h"
#include "IProcessManager.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class ProcessManager : public SmallObject<>, public IProcessManager
	{
	public:
		ProcessManager();
		~ProcessManager() override;

		void AttachProcess(ProcessPtr ptr) override;
		void UpdateProcesses(TimeDuration delta) override;
	private:
		ProcessManager(ProcessManager& rhs) = delete;
		ProcessManager& operator=(ProcessManager& rhs) = delete;

		ProcessManager(ProcessManager&& rhs) = delete;
		ProcessManager& operator=(ProcessManager&& rhs) = delete;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif
