#ifndef H_CLOCK
#define H_CLOCK

#include <memory>
#include "SmallObject.h"

namespace VeritasEngine
{
	class Clock: public SmallObject<>
	{
	public:
		explicit Clock(float startTime = 0.0f, float timeScale = 1.0f);
		~Clock();

		static void Init();

		void Update(float wallSeconds);
		float CalculateDelta(const Clock& other);

		bool GetIsPaused() const;
		void SetIsPaused(bool isPaused);

		static uintmax_t GetFrequency();

		void SingleStep();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif