#include "Clock.h"

#include "../VeritasEngineBase/ClockUtil.h"


struct VeritasEngine::Clock::Impl : VeritasEngine::SmallObject<>
{
	Impl(float startTime, float timeScale)
		: m_timeCycles{ SecondsToCycles(startTime) }, m_isPaused{ false }, m_timeScale{ timeScale }
	{

	}

	static float CyclesToSeconds(unsigned long long cycles)
	{
		return static_cast<float>(cycles) / m_frequency;
	}

	static uintmax_t SecondsToCycles(float seconds)
	{
		return static_cast<decltype(m_timeCycles)>(seconds * m_frequency);
	}

	static unsigned long long m_frequency;

	uintmax_t m_timeCycles;
	float m_timeScale;
	bool m_isPaused;
};

uintmax_t VeritasEngine::Clock::Impl::m_frequency;

VeritasEngine::Clock::Clock(float startTime, float timeScale)
	: m_impl(std::make_unique<Impl>(startTime, timeScale))
{

}

VeritasEngine::Clock::~Clock() = default;

void VeritasEngine::Clock::Init()
{
	Clock::Impl::m_frequency = ClockUtil::GetClockFrequency();
}

void VeritasEngine::Clock::Update(float wallSeconds)
{
	if (!m_impl->m_isPaused)
	{
		m_impl->m_timeCycles += static_cast<decltype(m_impl->m_timeCycles)>(m_impl->m_timeScale * m_impl->SecondsToCycles(wallSeconds));
	}
}

float VeritasEngine::Clock::CalculateDelta(const Clock& other)
{
	auto diff = m_impl->m_timeCycles - other.m_impl->m_timeCycles;

	return m_impl->CyclesToSeconds(diff);
}

bool VeritasEngine::Clock::GetIsPaused() const
{
	return m_impl->m_isPaused;
}

void VeritasEngine::Clock::SetIsPaused(bool isPaused)
{
	m_impl->m_isPaused = isPaused;
}

uintmax_t VeritasEngine::Clock::GetFrequency()
{
	return Impl::m_frequency;
}

void VeritasEngine::Clock::SingleStep()
{
	if (m_impl->m_isPaused)
	{
		auto seconds = 1.0f / 60.0f;

		Update(seconds);
	}
}