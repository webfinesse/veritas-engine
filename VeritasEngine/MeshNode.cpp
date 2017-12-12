#include "MeshNode.h"

#include "../VeritasEngineBase/MeshInfo.h"

struct VeritasEngine::MeshNode::Impl : public VeritasEngine::SmallObject<>
{
	Impl()
	{

	}

	Impl(SerializedMeshNode& serializedNode)
		: m_transform{ serializedNode.m_transform }, m_meshIndicies{ serializedNode.meshIndicies }, m_children{ serializedNode.m_children.begin(), serializedNode.m_children.end() }, m_jointIndex{ serializedNode.m_jointIndex }
	{

	}


	VeritasEngine::Matrix4x4 m_transform{};
	std::vector<unsigned int> m_meshIndicies{};
	std::vector<MeshNode> m_children{};
	int m_jointIndex{};
};

VeritasEngine::MeshNode::MeshNode()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::MeshNode::~MeshNode() = default;

VeritasEngine::MeshNode::MeshNode(SerializedMeshNode& serializedNode)
	: m_impl(std::make_unique<Impl>(serializedNode))
{

}

const VeritasEngine::Matrix4x4& VeritasEngine::MeshNode::GetTransform() const
{
	return m_impl->m_transform;
}

void VeritasEngine::MeshNode::SetTransform(const Matrix4x4& transform)
{
	m_impl->m_transform = transform;
}

const int VeritasEngine::MeshNode::GetJointIndex() const
{
	return m_impl->m_jointIndex;
}

const void VeritasEngine::MeshNode::SetJointIndex(int index)
{
	m_impl->m_jointIndex = index;
}

const std::vector<unsigned int>& VeritasEngine::MeshNode::GetMeshIndices() const
{
	return m_impl->m_meshIndicies;
}

void VeritasEngine::MeshNode::SetMeshIndices(const std::vector<unsigned int>& meshIndices)
{
	m_impl->m_meshIndicies = meshIndices;
}

const std::vector<VeritasEngine::MeshNode>& VeritasEngine::MeshNode::GetChildren() const
{
	return m_impl->m_children;
}

void VeritasEngine::MeshNode::AddChild(MeshNode&& child)
{
	m_impl->m_children.emplace_back(child);
}

VeritasEngine::MeshNode::MeshNode(const MeshNode& rhs)
	: m_impl(std::make_unique<Impl>(*rhs.m_impl))
{

}

VeritasEngine::MeshNode& VeritasEngine::MeshNode::operator=(const MeshNode& rhs)
{
	if (this != &rhs)
	{
		*m_impl = *rhs.m_impl;
	}

	return *this;
}
