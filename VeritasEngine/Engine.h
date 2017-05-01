#ifndef H_ENGINE
#define H_ENGINE

#include <memory>
#include "DynamicLibraryHelper.h"
#include "SmallObject.h"

using namespace std;

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
	class GamePropertyManager;

	class EXPORT Engine : public SmallObject<>
	{
	public:
		Engine(shared_ptr<IProcessManager> processManager, 
			   shared_ptr<IWorldSetup> worldSetup, 
			   shared_ptr<IRenderingServices> renderingServices, 
			   shared_ptr<IResourceManager> resourceManager,
			   shared_ptr<IGameClock> gameClock,
			   shared_ptr<GamePropertyManager> gamePropertyManager);

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
		GamePropertyManager& GetGamePropertyManager() const;
		IWorldSetup& GetWorldSetup() const;
		IResourceManager& GetResourceManager() const;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif