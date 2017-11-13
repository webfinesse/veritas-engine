#include <vector>

#include "MeshInstance.h"

#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "BufferIndicies.h"
#include "Engine.h"
#include "MeshSubset.h"
#include "MeshNode.h"
#include "Skeleton.h"
#include "../VeritasEngineBase/Animation.h"
#include "../VeritasEngineBase/ResourceHandle.h"


struct VeritasEngine::MeshInstance::Impl : SmallObject<>
{
	MeshNode m_root{};
	const ResourceHandle* m_skeleton{};
	IVertexBuffer* m_vertexBuffer{};
	IIndexBuffer* m_indexBuffer{};
	BufferIndicies m_vertexBufferIndicies{0, 0};
	BufferIndicies m_indexBufferIndicies{0, 0};
	std::vector<Animation> m_animation{};
	std::vector<MeshSubset> m_subsets{};
	Matrix4x4 m_globalInverseTransform;
};

VeritasEngine::MeshInstance::MeshInstance()
	: m_impl(std::make_unique<Impl>())
{
	
}

VeritasEngine::MeshInstance::~MeshInstance() = default;

VeritasEngine::MeshNode& VeritasEngine::MeshInstance::GetRootNode() const
{
	return m_impl->m_root;
}

VeritasEngine::MeshSubset& VeritasEngine::MeshInstance::CreateSubset(const SerializedMeshSubset& subset)
{
	m_impl->m_subsets.emplace_back(subset);
	return m_impl->m_subsets.back();
}

VeritasEngine::MeshSubset& VeritasEngine::MeshInstance::GetSubset(unsigned int index) const
{
	return m_impl->m_subsets[index];
}

std::size_t VeritasEngine::MeshInstance::GetSubsetCount() const
{
	return m_impl->m_subsets.size();
}

void VeritasEngine::MeshInstance::SetVertices(IVertexBuffer* vertexBuffer, unsigned char* verticies, std::size_t numOfVertices)
{
	m_impl->m_vertexBuffer = vertexBuffer;
	m_impl->m_vertexBufferIndicies = m_impl->m_vertexBuffer->AddVerticies(verticies, numOfVertices);
}

VeritasEngine::IVertexBuffer& VeritasEngine::MeshInstance::GetVertexBuffer() const
{
	return *m_impl->m_vertexBuffer;
}

std::size_t VeritasEngine::MeshInstance::GetVertexBufferStartIndex() const
{
	return m_impl->m_vertexBufferIndicies.StartIndex;
}

std::size_t VeritasEngine::MeshInstance::GetVertexSize() const
{
	return m_impl->m_vertexBuffer->GetVertexSize();
}

void VeritasEngine::MeshInstance::SetIndicies(IIndexBuffer* indexBuffer, unsigned int* indicies, std::size_t numOfIndicies)
{
	m_impl->m_indexBuffer = indexBuffer;
	m_impl->m_indexBufferIndicies = indexBuffer->AddIndicies(indicies, numOfIndicies);
}

VeritasEngine::IIndexBuffer& VeritasEngine::MeshInstance::GetIndexBuffer() const
{
	return *m_impl->m_indexBuffer;
}

std::size_t VeritasEngine::MeshInstance::GetIndexBufferStartIndex() const
{
	return m_impl->m_indexBufferIndicies.StartIndex;
}

void VeritasEngine::MeshInstance::SetSkeleton(const ResourceHandle* skeleton)
{
	m_impl->m_skeleton = skeleton;
}

const VeritasEngine::ResourceHandle* VeritasEngine::MeshInstance::GetSkeleton() const
{
	return m_impl->m_skeleton;
}

void VeritasEngine::MeshInstance::SetAnimations(const std::vector<Animation>& animation)
{
	m_impl->m_animation = animation;
}

const std::vector<VeritasEngine::Animation>& VeritasEngine::MeshInstance::GetAnimations() const
{
	return m_impl->m_animation;
}

void VeritasEngine::MeshInstance::SetGlobalInverseTransform(const Matrix4x4& transform)
{
	m_impl->m_globalInverseTransform = transform;
}

const VeritasEngine::Matrix4x4& VeritasEngine::MeshInstance::GetGlobalInverseTransform() const
{
	return m_impl->m_globalInverseTransform;
}


VeritasEngine::MeshInstance::MeshInstance(const MeshInstance& rhs)
	: m_impl(std::make_unique<Impl>(*rhs.m_impl))
{

}

VeritasEngine::MeshInstance& VeritasEngine::MeshInstance::operator=(const MeshInstance& rhs)
{
	if (this != &rhs)
	{
		*m_impl = *rhs.m_impl;
	}

	return *this;
}