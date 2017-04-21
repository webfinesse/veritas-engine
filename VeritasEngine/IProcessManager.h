#pragma once

#include "ClockUnits.h"
#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	class Process;
	using ProcessPtr = std::shared_ptr <Process>;

	class EXPORT IProcessManager
	{
	public:
		virtual ~IProcessManager() = default;
		virtual void AttachProcess(ProcessPtr ptr) = 0;
		virtual void UpdateProcesses(TimeDuration delta) = 0;
	};
}