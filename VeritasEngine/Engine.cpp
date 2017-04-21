#include "Engine.h"
#include "ProcessManager.h"
#include "Vertex.h"
#include "RenderingServices.h"
#include "Renderer.h"
#include "VertexBufferManager.h"
#include "Scene.h"
#include "IWorldSetup.h"

#include "MeshShader.h"
#include "ResourceManager.h"

#include "RendererProperties.h"

#include "GameClock.h"
#include "SkinnedVertex.h"

using namespace std;

struct VeritasEngine::Engine::Impl : public VeritasEngine::SmallObject<>
{
	Impl(shared_ptr<IProcessManager> processManager, shared_ptr<IWorldSetup> worldSetup, shared_ptr<IRenderingServices> renderingServices, shared_ptr<IResourceManager> resourceManager)
		: m_processManager{ std::move(processManager) }, m_worldSetup{ std::move(worldSetup) }, m_renderingServices{ std::move(renderingServices) }, m_resourceManager{ std::move(resourceManager) }, m_isInitialized { false }
	{

	}

	shared_ptr<GameClock> m_gameClock;
	shared_ptr<IProcessManager> m_processManager;
	shared_ptr<IWorldSetup> m_worldSetup;
	shared_ptr<IResourceManager> m_resourceManager;
	shared_ptr<IRenderingServices> m_renderingServices;

	float m_currentFps { 0 };
	bool m_isInitialized;
};

VeritasEngine::Engine::Engine(shared_ptr<IProcessManager> processManager, shared_ptr<IWorldSetup> worldSetup, shared_ptr<IRenderingServices> renderingServices, shared_ptr<IResourceManager> resourceManager)
	: m_impl(std::make_unique<Impl>(processManager, worldSetup, renderingServices, resourceManager))
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

VeritasEngine::IWorldSetup& VeritasEngine::Engine::GetWorldSetup() const
{
	return *m_impl->m_worldSetup;
}

VeritasEngine::IResourceManager& VeritasEngine::Engine::GetResourceManager() const
{
	return *m_impl->m_resourceManager;
}

void VeritasEngine::Engine::Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight)
{
	m_impl->m_gameClock = std::make_unique<GameClock>();
	
	m_impl->m_worldSetup->Init(*this);
	m_impl->m_renderingServices->GetRenderer().Init(osData, bufferWidth, bufferHeight);

	auto meshShader = std::make_shared<MeshShader>();
	meshShader->Init();

	m_impl->m_renderingServices->GetScene().SetMeshShader(meshShader);

	m_impl->m_renderingServices->GetVertexBufferManager().RegisterVertexFormat(Vertex::Type, sizeof(Vertex));
	m_impl->m_renderingServices->GetVertexBufferManager().RegisterVertexFormat(SkinnedVertex::Type, sizeof(SkinnedVertex));

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

	if (!m_impl->m_gameClock->GetIsPaused())
	{
		m_impl->m_processManager->UpdateProcesses(delta);
	}

	m_impl->m_renderingServices->GetScene().OnRender(m_impl->m_renderingServices->GetRenderer());
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

void VeritasEngine::Engine::Shutdown()
{
	m_impl->m_gameClock = nullptr;
	m_impl->m_processManager = nullptr;
	m_impl->m_resourceManager = nullptr;
	m_impl->m_renderingServices = nullptr;

	VeritasEngine::RendererProperties::ResourcedMesh.Clear();
	VeritasEngine::RendererProperties::ObjectMesh.Clear();
}