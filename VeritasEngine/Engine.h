#ifndef H_ENGINE
#define H_ENGINE

#include <memory>
#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/Singleton.h"
#include "SmallObject.h"

using namespace std;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class ResourceManager;
	class RenderingServices;
	class ProcessManager;

	class EXPORT Engine : public Singleton<Engine>, public SmallObject<>
	{
		friend class Singleton<Engine>;

	public:
		void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight);
		void Reinit(unsigned int bufferWidth, unsigned int bufferHeight);
		void Shutdown();
		void Loop();
		void TogglePause();
		void SetIsPaused(bool isPaused);
		bool IsInitialized() const;
		float GetCurrentFps() const;
		RenderingServices& GetRenderingServices() const;
		ProcessManager& GetProcessManager() const;
		ResourceManager& GetResourceManager() const;
	private:
		Engine();
		~Engine();

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif