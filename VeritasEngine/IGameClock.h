#pragma once

#include "ClockUnits.h"

namespace VeritasEngine
{
	class IGameClock
	{
	public:
		virtual ~IGameClock() = default;

		virtual TimeDuration GetDelta() = 0;
		virtual bool GetIsPaused() const = 0;
		virtual void SetIsPaused(bool isPaused) = 0;
	};
}