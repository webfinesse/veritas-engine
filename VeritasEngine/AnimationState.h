#ifndef H_ANIMATIONSTATE
#define H_ANIMATIONSTATE

#include "StringHash.h"
#include "AnimationClock.h"
#include <chrono>

namespace VeritasEngine
{
	
	struct AnimationState
	{
		AnimationState(GameObjectHandle handle, StringHash animationName, TimeDuration duration, TimeDuration startPoint, bool isLooped)
			: Handle { handle }, AnimationName{ animationName }, Clock{ duration, startPoint, 1, isLooped }
		{
			
		}

		AnimationState(const AnimationState&) = default;

		~AnimationState() = default;

		GameObjectHandle Handle; // 0 - 4
		StringHash AnimationName; // 4 - 8
		AnimationClock Clock; // 8 - 12 (32 bit) 8 - 16 (64 bit)
	};
}

#endif
