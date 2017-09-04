#include "Process.h"

struct VeritasEngine::Process::Impl : public VeritasEngine::SmallObject<>
{
	Impl()
		: m_status{ Process::Status::Uninitalized }
	{

	}

	Process::Status m_status;
};

VeritasEngine::Process::Process()
	: m_impl{ std::make_unique<Impl>() }
{

}

VeritasEngine::Process::~Process() = default;

VeritasEngine::Process::Status VeritasEngine::Process::GetStatus() const
{
	return m_impl->m_status;
}

void VeritasEngine::Process::Succeed()
{
	m_impl->m_status = Process::Status::Succeeded;
}

void VeritasEngine::Process::Fail()
{
	m_impl->m_status = Process::Status::Failed;
}

void VeritasEngine::Process::Pause()
{
	m_impl->m_status = Process::Status::Paused;
}

void VeritasEngine::Process::UnPause()
{
	m_impl->m_status = Process::Status::Running;
}

void VeritasEngine::Process::OnInitialized()
{
	m_impl->m_status = Process::Status::Running;
}

void VeritasEngine::Process::OnSuccess()
{

}

void VeritasEngine::Process::OnFail()
{

}

void VeritasEngine::Process::OnAbort()
{

}