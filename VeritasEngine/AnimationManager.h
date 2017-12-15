#ifndef H_ANIMATIONMANAGER
#define H_ANIMATIONMANAGER

#include <memory>

#include "IAnimationManager.h"

namespace VeritasEngine
{
	class GamePropertyManager;
	class IJobManager;

	class AnimationManager : public IAnimationManager
	{
	public:
		AnimationManager(std::shared_ptr<GamePropertyManager> gamePropertyManager, std::shared_ptr<IJobManager> jobManager);
		~AnimationManager() override = default;

		void AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped, float timeScale = 1) override;
		Job* CalculatePoses(TimeDuration update) override;
		const Matrix4x4* GetGlobalPoses(GameObjectHandle handle) override; 
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
