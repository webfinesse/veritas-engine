#include "RenderingServices.h"

#include "VertexBufferManager.h"
#include "IIndexBuffer.h"
#include "Scene.h"
#include "Renderer.h"

struct VeritasEngine::RenderingServices::Impl : public VeritasEngine::SmallObject<>
{
	Impl(std::shared_ptr<IScene> scene, std::shared_ptr<IIndexBuffer> indexBuffer)
		: m_renderer{ std::make_unique<Renderer>() },
		m_vertexBufferManager{ std::make_unique<VertexBufferManager>() },
		m_indexBuffer { indexBuffer },
		m_scene { scene }
	{

	}

	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<VertexBufferManager> m_vertexBufferManager;
	std::shared_ptr<IIndexBuffer> m_indexBuffer;
	std::shared_ptr<IScene> m_scene;
};

VeritasEngine::RenderingServices::RenderingServices(std::shared_ptr<IScene> scene, std::shared_ptr<IIndexBuffer> indexBuffer)
	: m_impl { std::make_unique<Impl>(scene, indexBuffer) }
{

}

VeritasEngine::RenderingServices::~RenderingServices() = default;

VeritasEngine::RenderingServices::RenderingServices(RenderingServices&& other) noexcept
	: m_impl{ std::move(other.m_impl) }
{
}

VeritasEngine::RenderingServices& VeritasEngine::RenderingServices::operator=(RenderingServices&& other) noexcept
{
	if(this != &other)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}

VeritasEngine::VertexBufferManager& VeritasEngine::RenderingServices::GetVertexBufferManager() const
{
	return *m_impl->m_vertexBufferManager;
}

VeritasEngine::IIndexBuffer& VeritasEngine::RenderingServices::GetIndexBuffer() const
{
	return *m_impl->m_indexBuffer;
}

VeritasEngine::IScene& VeritasEngine::RenderingServices::GetScene() const
{
	return *m_impl->m_scene;
}

VeritasEngine::Renderer& VeritasEngine::RenderingServices::GetRenderer() const
{
	return *m_impl->m_renderer;
}
