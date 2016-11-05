#pragma once
#include <chrono>

namespace VeritasEngine
{
	using FrameDuration = std::chrono::duration<float, std::ratio<1, 60>>;
	using TimeDuration = std::chrono::duration<float>;
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	constexpr FrameDuration OneFrame = FrameDuration(1);
}