#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RendererProperties.h"

#include "../VeritasEngineBase/RendererImpl.h"

struct VeritasEngine::Renderer::Impl 
{
public:
	Impl()
		: m_platformRenderer{}, m_aspectRatio{}
	{

	}

	VeritasEngine::RendererImpl m_platformRenderer;
	float m_aspectRatio;
};

VeritasEngine::Renderer::Renderer()
	: m_impl{ std::make_unique<Impl>() }
{

}

void VeritasEngine::Renderer::Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight)
{
	m_impl->m_aspectRatio = bufferWidth / static_cast<float>(bufferHeight);
	m_impl->m_platformRenderer.Init(osData, bufferWidth, bufferHeight);
}

void VeritasEngine::Renderer::Resize(unsigned int bufferWidth, unsigned int bufferHeight)
{
	m_impl->m_aspectRatio = bufferWidth / static_cast<float>(bufferHeight);
	m_impl->m_platformRenderer.Resize(bufferWidth, bufferHeight);
}

void VeritasEngine::Renderer::Clear()
{
	m_impl->m_platformRenderer.Clear();
}

void VeritasEngine::Renderer::Present()
{
	m_impl->m_platformRenderer.Present();
}

float VeritasEngine::Renderer::GetAspectRatio() const
{
	return m_impl->m_aspectRatio;
}

VeritasEngine::Renderer::~Renderer() = default;

void VeritasEngine::Renderer::RenderSubset(const MeshInstance& mesh, unsigned int subsetIndex) const
{
	auto& subset = mesh.GetSubset(subsetIndex);

	const unsigned int strides[1] = { static_cast<unsigned int>(subset.GetVertexSize()) };
	const unsigned int offsets[1] = { 0 };

	m_impl->m_platformRenderer.SetVertexBuffer(subset.GetVertexBuffer().GetNativeBuffer(), strides, offsets);

	auto baseVertexIndex = subset.GetVertexBufferBaseIndex();
				
	m_impl->m_platformRenderer.SetIndexBuffer(subset.GetIndexBuffer().GetNativeBuffer());
		
	m_impl->m_platformRenderer.DrawIndexed(subset.IndexCount(), subset.GetIndexOffset(), baseVertexIndex);
}