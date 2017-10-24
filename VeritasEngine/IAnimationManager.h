#ifndef H_IANIMATIONMANAGER
#define H_IANIMATIONMANAGER

#include "../VeritasEngineBase/GameObjectHandle.h"
#include "StringHash.h"

namespace VeritasEngine
{
	class IAnimationManager
	{
	public:
		virtual ~IAnimationManager() = default;

		virtual void AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped) = 0;
	};
}

#endif