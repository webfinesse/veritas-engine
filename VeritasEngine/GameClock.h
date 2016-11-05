#pragma once

#include <memory>
#include "ClockUnits.h"

namespace VeritasEngine
{
	class GameClock
	{
	public:
		explicit GameClock();
		~GameClock();

		TimeDuration GetDelta();
		bool GetIsPaused() const;
		void SetIsPaused(bool isPaused);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}