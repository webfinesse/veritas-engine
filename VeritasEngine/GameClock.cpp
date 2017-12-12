#include "GameClock.h"

struct VeritasEngine::GameClock::Impl : public SmallObject<>
{
	TimePoint m_currentTime { std::chrono::high_resolution_clock::now() };
	bool m_isPaused { false };
};

VeritasEngine::GameClock::GameClock()
	: m_impl(std::make_unique<Impl>())
{
	
}

VeritasEngine::GameClock::~GameClock() = default;

VeritasEngine::TimeDuration VeritasEngine::GameClock::GetDelta()
{
	const auto newTime = std::chrono::high_resolution_clock::now();

	const auto delta = std::chrono::duration_cast<TimeDuration>(newTime - m_impl->m_currentTime);

	m_impl->m_currentTime = newTime;

	return delta;
}

void VeritasEngine::GameClock::SetIsPaused(const bool isPaused)
{
	m_impl->m_isPaused = isPaused;
}


bool VeritasEngine::GameClock::GetIsPaused() const
{
	return m_impl->m_isPaused;
}



