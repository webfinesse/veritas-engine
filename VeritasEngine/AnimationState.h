#ifndef H_ANIMATIONSTATE
#define H_ANIMATIONSTATE

#include "StringHash.h"
#include "AnimationClock.h"
#include "../VeritasEngineBase/MathTypes.h"
#include <chrono>
#include "Skeleton.h"

namespace VeritasEngine
{
	
	struct AnimationState
	{
		AnimationState(GameObjectHandle handle, StringHash animationName, TimeDuration duration, TimeDuration startPoint, bool isLooped, float timeScale = 1)
			: Handle { handle }, AnimationName{ animationName }, Clock{ duration, startPoint, timeScale, isLooped }
		{
			
		}

		AnimationState(const AnimationState&) = default;

		~AnimationState() = default;

		GameObjectHandle Handle; // 0 - 4
		StringHash AnimationName; // 4 - 8
		AnimationClock Clock; // 8 - 12 (32 bit) 8 - 16 (64 bit)
		Matrix4x4 LocalPoses[MAX_JOINTS]; // 16 - 8208,  64 * 128 = 8192
		Matrix4x4 GlobalPoses[MAX_JOINTS]; // 8208 - 16400,  64 * 128 = 8192
	};
}

#endif
