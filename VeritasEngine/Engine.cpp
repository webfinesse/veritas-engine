#include "Engine.h"

#include "IRenderingServices.h"
#include "IWorldSetup.h"
#include "IProcessManager.h"
#include "IResourceManager.h"
#include "IGameClock.h"
#include "IAnimationManager.h"
#include "IJobManager.h"

#include "IRenderer.h"
#include "VertexBufferManager.h"
#include "Scene.h"

#include "SkinnedVertex.h"
#include "Vertex.h"

#include "FrameDescription.h"

struct VeritasEngine::Engine::Impl : public VeritasEngine::SmallObject<>
{
	Impl(std::shared_ptr<IProcessManager>&& processManager, 
		 std::shared_ptr<IWorldSetup>&& worldSetup, 
		 std::shared_ptr<IRenderingServices>&& renderingServices, 
	     std::shared_ptr<IResourceManager>&& resourceManager,
		 std::shared_ptr<IGameClock>&& gameClock,
		 std::shared_ptr<GamePropertyManager>&& gamePropertyManager,
		 std::shared_ptr<IAnimationManager>&& animationManager,
		 std::shared_ptr<IJobManager>&& jobManager)
		: m_gameClock{ std::move(gameClock) }, m_processManager{ std::move(processManager) }, m_worldSetup{ std::move(worldSetup) }, m_resourceManager{ std::move(resourceManager) }, m_renderingServices{ std::move(renderingServices) }, m_gamePropertyManager { std::move(gamePropertyManager) }, m_animationManager{ std::move(animationManager) }, m_jobManager { std::move(jobManager) }
	{

	}

	std::shared_ptr<IGameClock> m_gameClock;
	std::shared_ptr<IProcessManager> m_processManager;
	std::shared_ptr<IWorldSetup> m_worldSetup;
	std::shared_ptr<IResourceManager> m_resourceManager;
	std::shared_ptr<IRenderingServices> m_renderingServices;
	std::shared_ptr<GamePropertyManager> m_gamePropertyManager;
	std::shared_ptr<IAnimationManager> m_animationManager;
	std::shared_ptr<IJobManager> m_jobManager;

	float m_currentFps { 0 };
	bool m_isInitialized { false };
	FrameDescription m_frameDesc {};
};

VeritasEngine::Engine::Engine(std::shared_ptr<IProcessManager> processManager, 
							  std::shared_ptr<IWorldSetup> worldSetup, 
						      std::shared_ptr<IRenderingServices> renderingServices, 
							  std::shared_ptr<IResourceManager> resourceManager,
						      std::shared_ptr<IGameClock> gameClock,
							  std::shared_ptr<GamePropertyManager> gamePropertyManager,
							  std::shared_ptr<IAnimationManager> animationManager,
							  std::shared_ptr<IJobManager> jobManager)
	: m_impl{ std::make_unique<Impl>(std::move(processManager), std::move(worldSetup), std::move(renderingServices), std::move(resourceManager), std::move(gameClock), std::move(gamePropertyManager), std::move(animationManager), std::move(jobManager)) }
{
	
}

VeritasEngine::Engine::Engine(Engine&& other) noexcept
	: m_impl { std::move(other.m_impl) }
{
}

VeritasEngine::Engine& VeritasEngine::Engine::operator=(Engine&& other) noexcept
{
	if(this != &other)
	{
		this->m_impl = std::move(other.m_impl);
	}

	return *this;
}

VeritasEngine::Engine::~Engine() = default;

VeritasEngine::IRenderingServices& VeritasEngine::Engine::GetRenderingServices() const
{
	return *m_impl->m_renderingServices;
}

VeritasEngine::IProcessManager& VeritasEngine::Engine::GetProcessManager() const
{
	return *m_impl->m_processManager;
}

VeritasEngine::IAnimationManager& VeritasEngine::Engine::GetAnimationManager() const
{
	return *m_impl->m_animationManager;
}

VeritasEngine::GamePropertyManager& VeritasEngine::Engine::GetGamePropertyManager() const
{
	return *m_impl->m_gamePropertyManager;
}

VeritasEngine::IWorldSetup& VeritasEngine::Engine::GetWorldSetup() const
{
	return *m_impl->m_worldSetup;
}

VeritasEngine::IResourceManager& VeritasEngine::Engine::GetResourceManager() const
{
	return *m_impl->m_resourceManager;
}

VeritasEngine::IJobManager& VeritasEngine::Engine::GetJobManager() const
{
	return *m_impl->m_jobManager;
}

void VeritasEngine::Engine::Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight)
{
	m_impl->m_worldSetup->Init(*this);
	m_impl->m_renderingServices->GetRenderer().Init(osData, bufferWidth, bufferHeight, m_impl->m_resourceManager);
	m_impl->m_renderingServices->GetScene().Init(m_impl->m_resourceManager);
	m_impl->m_animationManager->Init(m_impl->m_resourceManager);

	m_impl->m_renderingServices->GetVertexBufferManager().RegisterVertexFormat(Vertex::Type, sizeof(Vertex));
	m_impl->m_renderingServices->GetVertexBufferManager().RegisterVertexFormat(SkinnedVertex::Type, sizeof(SkinnedVertex));

	m_impl->m_jobManager->Init();

	m_impl->m_isInitialized = true;
}

void VeritasEngine::Engine::Reinit(unsigned int bufferWidth, unsigned int bufferHeight)
{
	m_impl->m_renderingServices->GetRenderer().Resize(bufferWidth, bufferHeight);
}

void VeritasEngine::Engine::Loop()
{
	auto delta = m_impl->m_gameClock->GetDelta();

	m_impl->m_currentFps = 1 / delta.count();

	if (delta > OneFrame) {
		delta = OneFrame;
	}

	Job* animationManagerJob{ nullptr };

	if (!m_impl->m_gameClock->GetIsPaused())
	{
		m_impl->m_processManager->UpdateProcesses(delta);
		animationManagerJob = m_impl->m_animationManager->CalculatePoses(delta);
	}

	m_impl->m_frameDesc.StaticObjects.resize(0);
	m_impl->m_frameDesc.AnimatedObjects.resize(0);

	auto& renderer = m_impl->m_renderingServices->GetRenderer();
	m_impl->m_frameDesc.AspectRatio = renderer.GetAspectRatio();

	if(animationManagerJob)
	{
		m_impl->m_jobManager->Wait(animationManagerJob);
	}

	m_impl->m_renderingServices->GetScene().OnRender(m_impl->m_frameDesc);
	renderer.Render(m_impl->m_frameDesc);
}

void VeritasEngine::Engine::TogglePause()
{
	m_impl->m_gameClock->SetIsPaused(!m_impl->m_gameClock->GetIsPaused());
}

void VeritasEngine::Engine::SetIsPaused(bool isPaused)
{
	m_impl->m_gameClock->SetIsPaused(isPaused);
}

bool VeritasEngine::Engine::IsInitialized() const
{
	return m_impl->m_isInitialized;
}

float VeritasEngine::Engine::GetCurrentFps() const
{
	return m_impl->m_currentFps;
}