#include "RenderingServices.h"

#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "Scene.h"
#include "Renderer.h"

struct VeritasEngine::RenderingServices::Impl : public VeritasEngine::SmallObject<>
{
	Impl()
		: m_renderer{ std::make_unique<Renderer>() },
		m_vertexBufferManager{ std::make_unique<VertexBufferManager>() },
		m_indexBufferManager { std::make_unique<IndexBufferManager>() },
		m_scene { std::make_unique<Scene>() }
	{

	}

	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<VertexBufferManager> m_vertexBufferManager;
	std::unique_ptr<IndexBufferManager> m_indexBufferManager;
	std::unique_ptr<Scene> m_scene;
};

VeritasEngine::RenderingServices::RenderingServices()
	: m_impl { std::make_unique<Impl>() }
{

}

VeritasEngine::RenderingServices::~RenderingServices() = default;

VeritasEngine::VertexBufferManager& VeritasEngine::RenderingServices::GetVertexBufferManager() const
{
	return *m_impl->m_vertexBufferManager;
}

VeritasEngine::IndexBufferManager& VeritasEngine::RenderingServices::GetIndexBufferManager() const
{
	return *m_impl->m_indexBufferManager;
}

VeritasEngine::Scene& VeritasEngine::RenderingServices::GetScene() const
{
	return *m_impl->m_scene;
}

VeritasEngine::Renderer& VeritasEngine::RenderingServices::GetRenderer() const
{
	return *m_impl->m_renderer;
}
