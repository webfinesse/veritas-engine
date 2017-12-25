#include "MeshSubset.h"

#include "BufferIndicies.h"
#include "../VeritasEngineBase/MeshInfo.h"

struct VeritasEngine::MeshSubset::Impl : public VeritasEngine::SmallObject<>
{
	Impl(const SerializedMeshSubset& subset)
		: m_indexBufferIndicies{ subset.m_indexBaseIndex, subset.m_indexCount }, m_vertexBufferIndicies{subset.m_vertexBaseIndex, subset.m_vertexCount }
	{
		
	}

	BufferIndicies m_indexBufferIndicies;
	BufferIndicies m_vertexBufferIndicies;
	ResourceHandle m_material{};
};

VeritasEngine::MeshSubset::MeshSubset(const SerializedMeshSubset& subset)
	: m_impl(std::make_unique<Impl>(subset))
{

}

VeritasEngine::MeshSubset::~MeshSubset() = default;

const VeritasEngine::BufferIndicies& VeritasEngine::MeshSubset::GetVertexBufferIndicies() const
{
	return m_impl->m_vertexBufferIndicies;
}

const VeritasEngine::BufferIndicies& VeritasEngine::MeshSubset::GetIndexBufferIndicies() const
{
	return m_impl->m_indexBufferIndicies;
}

VeritasEngine::ResourceHandle VeritasEngine::MeshSubset::GetMaterial() const
{
	return m_impl->m_material;
}

void VeritasEngine::MeshSubset::SetMaterial(ResourceHandle material)
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