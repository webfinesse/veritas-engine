#ifndef H_ENGINE
#define H_ENGINE

#include <memory>
#include "DynamicLibraryHelper.h"
#include "SmallObject.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class IResourceManager;
	class IRenderingServices;
	class IProcessManager;
	class IResourceManager;
	class IWorldSetup;
	class IGameClock;
	class IAnimationManager;
	class GamePropertyManager;
	class IJobManager;

	class EXPORT Engine : public SmallObject<>
	{
	public:
		Engine(std::shared_ptr<IProcessManager> processManager, 
			   std::shared_ptr<IWorldSetup> worldSetup, 
			   std::shared_ptr<IRenderingServices> renderingServices, 
			   std::shared_ptr<IResourceManager> resourceManager,
			   std::shared_ptr<IGameClock> gameClock,
			   std::shared_ptr<GamePropertyManager> gamePropertyManager,
			   std::shared_ptr<IAnimationManager> animationManager,
			   std::shared_ptr<IJobManager> jobManager);

		Engine(Engine&& other) noexcept;
		Engine& operator=(Engine&& other) noexcept;
		~Engine();

		void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight);
		void Reinit(unsigned int bufferWidth, unsigned int bufferHeight);
		void Loop();
		void TogglePause();
		void SetIsPaused(bool isPaused);
		bool IsInitialized() const;
		float GetCurrentFps() const;
		IRenderingServices& GetRenderingServices() const;
		IProcessManager& GetProcessManager() const;
		IAnimationManager& GetAnimationManager() const;
		GamePropertyManager& GetGamePropertyManager() const;
		IWorldSetup& GetWorldSetup() const;
		IResourceManager& GetResourceManager() const;
		IJobManager& GetJobManager() const;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif