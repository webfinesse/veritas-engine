#include "AnimationClock.h"
#include <algorithm>

struct VeritasEngine::AnimationClock::Impl : SmallObject<>
{
	Impl(TimeDuration duration, TimeDuration startTime, float timeScale, bool isLooped)
		: m_duration { duration }, m_currentTime { startTime }, m_timeScale { timeScale }, m_isLooped { isLooped }, m_isPaused { false }
	{
		
	}

	TimeDuration m_duration;
	TimeDuration m_currentTime;
	float m_timeScale;
	bool m_isLooped;
	bool m_isPaused;
};

VeritasEngine::AnimationClock::AnimationClock(TimeDuration duration, TimeDuration startTime, float timeScale, bool isLooped)
	: m_impl(std::make_unique<Impl>(duration, startTime, timeScale, isLooped))
{
}

VeritasEngine::AnimationClock::AnimationClock(const AnimationClock& other)
	: m_impl{ std::make_unique<Impl>(*other.m_impl) }
{
}

VeritasEngine::AnimationClock::AnimationClock(AnimationClock&& other) noexcept
	: m_impl{ std::move(other.m_impl) }
{
}

VeritasEngine::AnimationClock::~AnimationClock() = default;

void VeritasEngine::AnimationClock::Update(TimeDuration duration)
{
	if (!m_impl->m_isPaused) 
	{
		m_impl->m_currentTime += duration * m_impl->m_timeScale;
	}
}

float VeritasEngine::AnimationClock::GetPercentComplete() const
{
	return m_impl->m_isLooped
		? (fmod(m_impl->m_currentTime.count(), m_impl->m_duration.count()) / m_impl->m_duration.count())
		: std::min(1.0f, m_impl->m_currentTime.count() / m_impl->m_duration.count());
}

bool VeritasEngine::AnimationClock::GetIsPaused() const
{
	return m_impl->m_isPaused;
}

void VeritasEngine::AnimationClock::SetIsPaused(bool isPaused)
{
	m_impl->m_isPaused = isPaused;
}

bool VeritasEngine::AnimationClock::GetIsLooped() const
{
	return m_impl->m_isLooped;
}

void VeritasEngine::AnimationClock::SetIsLooped(bool isLooped)
{
	m_impl->m_isLooped = isLooped;
}

const VeritasEngine::TimeDuration& VeritasEngine::AnimationClock::GetDuration() const
{
	return m_impl->m_duration;
}

VeritasEngine::TimeDuration VeritasEngine::AnimationClock::GetCurrentTimeDuration() const
{
	return TimeDuration{ fmod(m_impl->m_currentTime.count(), m_impl->m_duration.count()) };
}

VeritasEngine::AnimationClock& VeritasEngine::AnimationClock::operator=(const AnimationClock& other)
{
	if(this != &other)
	{
		*m_impl = *other.m_impl;
	}

	return *this;
}

VeritasEngine::AnimationClock& VeritasEngine::AnimationClock::operator=(AnimationClock&& other) noexcept
{
	if (this != &other)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}
