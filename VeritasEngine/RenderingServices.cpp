#include "RenderingServices.h"

#include "IVertexBufferManager.h"
#include "IIndexBuffer.h"
#include "IScene.h"
#include "IRenderer.h"

struct VeritasEngine::RenderingServices::Impl : public VeritasEngine::SmallObject<>
{
	Impl(std::shared_ptr<IScene> scene, std::shared_ptr<IIndexBuffer> indexBuffer, std::shared_ptr<IVertexBufferManager> vertexBufferManager, std::shared_ptr<IRenderer> renderer)
		: m_renderer{ renderer },
		m_vertexBufferManager{ vertexBufferManager },
		m_indexBuffer { indexBuffer },
		m_scene { scene }
	{

	}

	std::shared_ptr<IRenderer> m_renderer;
	std::shared_ptr<IVertexBufferManager> m_vertexBufferManager;
	std::shared_ptr<IIndexBuffer> m_indexBuffer;
	std::shared_ptr<IScene> m_scene;
};

VeritasEngine::RenderingServices::RenderingServices(std::shared_ptr<IScene> scene, std::shared_ptr<IIndexBuffer> indexBuffer, std::shared_ptr<IVertexBufferManager> vertexBufferManager, std::shared_ptr<IRenderer> renderer)
	: m_impl { std::make_unique<Impl>(scene, indexBuffer, vertexBufferManager, renderer) }
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

VeritasEngine::IVertexBufferManager& VeritasEngine::RenderingServices::GetVertexBufferManager() const
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

VeritasEngine::IRenderer& VeritasEngine::RenderingServices::GetRenderer() const
{
	return *m_impl->m_renderer;
}
