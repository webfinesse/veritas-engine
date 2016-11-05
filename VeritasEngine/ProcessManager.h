#ifndef H_PROCESSMANAGER
#define H_PROCESSMANAGER

#include <memory>
#include "Process.h"
#include "SmallObject.h"
#include "DynamicLibraryHelper.h"
#include "ClockUnits.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	using ProcessPtr = std::shared_ptr <Process>;

	class EXPORT ProcessManager : public SmallObject<>
	{
	public:
		ProcessManager();
		~ProcessManager();

		void AttachProcess(ProcessPtr ptr);
		void UpdateProcesses(TimeDuration delta);
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
