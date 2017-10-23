#pragma once

#include <memory>
#include "SmallObject.h"
#include "IGameClock.h"

namespace VeritasEngine
{
	class GameClock: public SmallObject<>, public IGameClock
	{
	public:
		GameClock();
		~GameClock() override;

		TimeDuration GetDelta() override;
		bool GetIsPaused() const override;
		void SetIsPaused(const bool isPaused) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
