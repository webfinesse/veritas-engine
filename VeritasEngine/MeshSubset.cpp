#include "MeshSubset.h"

#include "../VeritasEngineBase/ResourceHandle.h"

#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "BufferIndicies.h"

struct VeritasEngine::MeshSubset::Impl : public VeritasEngine::SmallObject<>
{
	Impl()
		: m_vertexBuffer{ nullptr }, m_indexBuffer{ nullptr }, m_vertexBufferIndicies { 0, 0 }, m_indexBufferIndicies{ 0, 0 }
	{
		
	}

	IVertexBuffer* m_vertexBuffer;
	IIndexBuffer* m_indexBuffer;
	BufferIndicies m_vertexBufferIndicies;
	BufferIndicies m_indexBufferIndicies;
	const ResourceHandle* m_material;
};

VeritasEngine::MeshSubset::MeshSubset()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::MeshSubset::~MeshSubset()
{

}

void VeritasEngine::MeshSubset::SetVertices(IVertexBuffer* vertexBuffer, unsigned char* verticies, size_t numOfVertices)
{
	m_impl->m_vertexBuffer = vertexBuffer;
	m_impl->m_vertexBufferIndicies = m_impl->m_vertexBuffer->AddVerticies(verticies, numOfVertices);
}

size_t VeritasEngine::MeshSubset::GetVertexCount() const
{
	return m_impl->m_vertexBufferIndicies.NumberOfElements;
}

std::size_t VeritasEngine::MeshSubset::GetVertexSize() const
{
	return m_impl->m_vertexBuffer->GetVertexSize();
}

VeritasEngine::IVertexBuffer& VeritasEngine::MeshSubset::GetVertexBuffer() const
{
	return *m_impl->m_vertexBuffer;
}

size_t VeritasEngine::MeshSubset::GetVertexBufferBaseIndex() const {
	return m_impl->m_vertexBufferIndicies.StartIndex;
}

const VeritasEngine::BufferIndicies& VeritasEngine::MeshSubset::GetVertexBufferIndicies() const
{
	return m_impl->m_vertexBufferIndicies;
}

void VeritasEngine::MeshSubset::SetIndicies(IIndexBuffer* indexBuffer, unsigned int* indicies, size_t numOfIndicies)
{
	m_impl->m_indexBuffer = indexBuffer;
	m_impl->m_indexBufferIndicies = indexBuffer->AddIndicies(indicies, numOfIndicies);
}

size_t VeritasEngine::MeshSubset::GetIndexOffset() const
{
	return m_impl->m_indexBufferIndicies.StartIndex;
}

const VeritasEngine::BufferIndicies& VeritasEngine::MeshSubset::GetIndexBufferIndicies() const
{
	return m_impl->m_indexBufferIndicies;
}

size_t VeritasEngine::MeshSubset::IndexCount() const
{
	return m_impl->m_indexBufferIndicies.NumberOfElements;
}

VeritasEngine::IIndexBuffer& VeritasEngine::MeshSubset::GetIndexBuffer() const
{
	return *m_impl->m_indexBuffer;
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