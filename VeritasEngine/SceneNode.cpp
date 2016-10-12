#include "SceneNode.h"

#include <vector>
#include <algorithm>

struct VeritasEngine::SceneNode::Impl : VeritasEngine::SmallObject<>
{
	Impl(const GameObjectHandle handle)
		: m_handle{ handle }, m_children{}
	{

	}

	GameObjectHandle m_handle;
	std::vector<SceneNode> m_children;
};

VeritasEngine::SceneNode::SceneNode(const GameObjectHandle handle)
	: m_impl{ std::make_unique<Impl>(handle) }
{

}

VeritasEngine::SceneNode::~SceneNode() = default;

VeritasEngine::SceneNode::SceneNode(const SceneNode& rhs)
	: m_impl{ std::make_unique<Impl>(*rhs.m_impl) }
{

}

VeritasEngine::SceneNode& VeritasEngine::SceneNode::operator=(const VeritasEngine::SceneNode& rhs)
{
	if (this != &rhs)
	{
		*m_impl = *rhs.m_impl;
	}

	return *this;
}

const VeritasEngine::GameObjectHandle VeritasEngine::SceneNode::GetHandle() const
{
	return m_impl->m_handle;
}

void VeritasEngine::SceneNode::AddChild(const SceneNode& child)
{
	m_impl->m_children.push_back(child);
}

void VeritasEngine::SceneNode::RemoveChild(const GameObjectHandle handle)
{
	std::remove_if(m_impl->m_children.begin(), m_impl->m_children.end(), [&handle](SceneNode& node) { return node.m_impl->m_handle == handle; });
}