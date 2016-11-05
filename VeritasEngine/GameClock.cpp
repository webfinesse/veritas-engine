#include "GameClock.h"

struct VeritasEngine::GameClock::Impl
{
	Impl()
		: m_currentTime { std::chrono::high_resolution_clock::now() }, m_isPaused { false }
	{
		
	}

	TimePoint m_currentTime;
	bool m_isPaused;
};

VeritasEngine::GameClock::GameClock()
	: m_impl(std::make_unique<Impl>())
{
	
}

VeritasEngine::GameClock::~GameClock() = default;

VeritasEngine::TimeDuration VeritasEngine::GameClock::GetDelta()
{
	auto newTime = std::chrono::high_resolution_clock::now();

	auto delta = std::chrono::duration_cast<TimeDuration>(newTime - m_impl->m_currentTime);

	m_impl->m_currentTime = newTime;

	return delta;
}

void VeritasEngine::GameClock::SetIsPaused(bool isPaused)
{
	m_impl->m_isPaused = isPaused;
}


bool VeritasEngine::GameClock::GetIsPaused() const
{
	return m_impl->m_isPaused;
}



