#include "Engine.h"

#include "IRenderingServices.h"
#include "IWorldSetup.h"
#include "IProcessManager.h"
#include "IResourceManager.h"
#include "IGameClock.h"

#include "IRenderer.h"
#include "VertexBufferManager.h"
#include "Scene.h"
#include "IMeshShader.h"

#include "SkinnedVertex.h"
#include "Vertex.h"

#include "RendererProperties.h"

using namespace std;

struct VeritasEngine::Engine::Impl : public VeritasEngine::SmallObject<>
{
	Impl(shared_ptr<IProcessManager> processManager, 
		 shared_ptr<IWorldSetup> worldSetup, 
		 shared_ptr<IRenderingServices> renderingServices, 
	     shared_ptr<IResourceManager> resourceManager,
		 shared_ptr<IGameClock> gameClock,
		 shared_ptr<IMeshShader> meshShader)
		: m_gameClock{ std::move(gameClock) }, m_processManager{ std::move(processManager) }, m_worldSetup{ std::move(worldSetup) }, m_resourceManager{ std::move(resourceManager) }, m_renderingServices{ std::move(renderingServices) }, m_meshShader{ meshShader }
	{

	}

	shared_ptr<IGameClock> m_gameClock;
	shared_ptr<IProcessManager> m_processManager;
	shared_ptr<IWorldSetup> m_worldSetup;
	shared_ptr<IResourceManager> m_resourceManager;
	shared_ptr<IRenderingServices> m_renderingServices;
	shared_ptr<IMeshShader> m_meshShader;

	float m_currentFps { 0 };
	bool m_isInitialized { false };
};

VeritasEngine::Engine::Engine(shared_ptr<IProcessManager> processManager, 
							  shared_ptr<IWorldSetup> worldSetup, 
						      shared_ptr<IRenderingServices> renderingServices, 
							  shared_ptr<IResourceManager> resourceManager,
						      shared_ptr<IGameClock> gameClock,
							  shared_ptr<IMeshShader> meshShader)
	: m_impl(std::make_unique<Impl>(processManager, worldSetup, renderingServices, resourceManager, gameClock, meshShader))
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

VeritasEngine::Engine::~Engine()
{
	VeritasEngine::RendererProperties::ResourcedMesh.Clear();
	VeritasEngine::RendererProperties::ObjectMesh.Clear();
};

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
	m_impl->m_worldSetup->Init(*this);
	m_impl->m_renderingServices->GetRenderer().Init(osData, bufferWidth, bufferHeight);

	
	m_impl->m_meshShader->Init();

	m_impl->m_renderingServices->GetScene().SetMeshShader(m_impl->m_meshShader);

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