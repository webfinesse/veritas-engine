#pragma once

#include <memory>
#include "SmallObject.h"
#include "ClockUnits.h"
#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
#pragma warning(push)
#pragma warning(disable: 4251)

	class EXPORT AnimationClock : public SmallObject<>
	{
	public:
		AnimationClock(TimeDuration duration, TimeDuration startTime = std::chrono::seconds(0), float timeScale = 1.0f, bool isLooped = false);
		AnimationClock(const AnimationClock& other);
		AnimationClock(AnimationClock&& other) noexcept;
		~AnimationClock();

		void Update(TimeDuration duration);
		float GetPercentComplete() const;
		
		bool GetIsPaused() const;
		void SetIsPaused(bool isPaused);

		bool GetIsLooped() const;
		void SetIsLooped(bool isLooped);

		const TimeDuration& GetDuration() const;
		TimeDuration GetCurrentTime() const;

		AnimationClock& operator=(const AnimationClock& other);
		AnimationClock& operator=(AnimationClock&& other) noexcept;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

#pragma warning(pop)
}


