#include "MeshSubset.h"

#include "../VeritasEngineBase/ResourceHandle.h"

#include "VertexBuffer.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "IndexBuffer.h"
#include "BufferIndicies.h"
#include "Engine.h"
#include "RenderingServices.h"
#include "IndexBufferManager.h"

struct VeritasEngine::MeshSubset::Impl : public VeritasEngine::SmallObject<>
{
	Impl()
		: m_vertexBuffer{ nullptr }, m_vertexBufferIndicies{ 0, 0 }, m_indexBufferIndicies { 0, 0 }
	{
		
	}

	std::shared_ptr<VeritasEngine::VertexBuffer> m_vertexBuffer;
	VeritasEngine::BufferIndicies m_vertexBufferIndicies;
	VeritasEngine::BufferIndicies m_indexBufferIndicies;
	const ResourceHandle* m_material;
};

VeritasEngine::MeshSubset::MeshSubset()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::MeshSubset::~MeshSubset()
{

}

void VeritasEngine::MeshSubset::SetVertices(VertexTypeHandle handle, unsigned char* verticies, unsigned int numOfVertices)
{
	m_impl->m_vertexBuffer = VeritasEngine::Engine::Instance().GetRenderingServices().GetVertexBufferManager().GetBuffer(handle);
	m_impl->m_vertexBufferIndicies = m_impl->m_vertexBuffer->AddVerticies(verticies, numOfVertices);
}

unsigned int VeritasEngine::MeshSubset::GetVertexCount() const
{
	return m_impl->m_vertexBufferIndicies.NumberOfElements;
}

std::size_t VeritasEngine::MeshSubset::GetVertexSize() const
{
	return m_impl->m_vertexBuffer->GetVertexSize();
}

VeritasEngine::VertexBuffer& VeritasEngine::MeshSubset::GetVertexBuffer() const
{
	return *m_impl->m_vertexBuffer;
}

unsigned int VeritasEngine::MeshSubset::GetVertexBufferBaseIndex() const {
	return m_impl->m_vertexBufferIndicies.StartIndex;
}

void VeritasEngine::MeshSubset::SetIndicies(unsigned int* indicies, unsigned int numOfIndicies)
{
	m_impl->m_indexBufferIndicies = VeritasEngine::Engine::Instance().GetRenderingServices().GetIndexBufferManager().GetBuffer().AddIndicies(indicies, numOfIndicies);
}

unsigned int VeritasEngine::MeshSubset::GetIndexOffset() const
{
	return m_impl->m_indexBufferIndicies.StartIndex;
}

unsigned int VeritasEngine::MeshSubset::IndexCount() const
{
	return m_impl->m_indexBufferIndicies.NumberOfElements;
}

VeritasEngine::IndexBuffer& VeritasEngine::MeshSubset::GetIndexBuffer() const
{
	auto& manager = VeritasEngine::Engine::Instance().GetRenderingServices().GetIndexBufferManager();
	return manager.GetBuffer();
}

const VeritasEngine::ResourceHandle* const VeritasEngine::MeshSubset::GetMaterial() const
{
	return m_impl->m_material;
}

void VeritasEngine::MeshSubset::SetMaterial(const ResourceHandle* const material)
{
	m_impl->m_material = material;
}

VeritasEngine::MeshSubset::MeshSubset(const MeshSubset& rhs)
	: m_impl(std::make_unique<Impl>(*rhs.m_impl))
{

}

VeritasEngine::MeshSubset& VeritasEngine::MeshSubset::operator=(MeshSubset& rhs)
{
	if (this != &rhs)
	{
		*m_impl = *rhs.m_impl;
	}

	return *this;
}