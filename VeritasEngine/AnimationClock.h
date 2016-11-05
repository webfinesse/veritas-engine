#pragma once

#include <memory>
#include "SmallObject.h"
#include "ClockUnits.h"
#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	class EXPORT AnimationClock : SmallObject<>
	{
	public:
		AnimationClock(TimeDuration duration, TimeDuration startTime = std::chrono::seconds(0), float timeScale = 1.0f, bool isLooped = false);
		~AnimationClock();

		void Update(TimeDuration duration);
		float GetPercentComplete() const;
		
		bool GetIsPaused() const;
		void SetIsPaused(bool isPaused);

		bool GetIsLooped() const;
		void SetIsLooped(bool isLooped);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}


