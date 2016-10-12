#include "ProcessManager.h"
#include <list>

struct VeritasEngine::ProcessManager::Impl: public VeritasEngine::SmallObject<>
{
	Impl()
		: m_processes{}
	{

	}

	std::list<ProcessPtr> m_processes;
};

VeritasEngine::ProcessManager::ProcessManager()
	: m_impl{ std::make_unique<Impl>() }
{

}

VeritasEngine::ProcessManager::~ProcessManager() = default;

void VeritasEngine::ProcessManager::AttachProcess(ProcessPtr ptr)
{
	m_impl->m_processes.emplace_back(ptr);
}

void VeritasEngine::ProcessManager::UpdateProcesses(float delta)
{
	auto it = m_impl->m_processes.begin();
	while (it != m_impl->m_processes.end())
	{
		auto& currentProcess = *it;
		bool remove = false;

		auto currentIterator = it;
		++it;

		switch (currentProcess->GetStatus())
		{
		case Process::Status::Uninitalized:
			currentProcess->OnInitialized();
			break;

		case Process::Status::Running:
			currentProcess->OnUpdate(delta);
			break;

		case Process::Status::Aborted:
			currentProcess->OnAbort();
			remove = true;
			break;

		case Process::Status::Failed:
			currentProcess->OnFail();
			remove = true;
			break;

		case Process::Status::Succeeded:
			currentProcess->OnSuccess();
			remove = true;
			break;
		}

		if (remove)
		{
			m_impl->m_processes.erase(currentIterator);
		}
	}
}

