﻿#ifndef H_ANIMATIONMANAGER
#define H_ANIMATIONMANAGER

#include <memory>

#include "IAnimationManager.h"

namespace VeritasEngine
{
	class GamePropertyManager;

	class AnimationManager : public IAnimationManager
	{
	public:
		AnimationManager(std::shared_ptr<GamePropertyManager> gamePropertyManager);
		~AnimationManager() override = default;

		void AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped) override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif