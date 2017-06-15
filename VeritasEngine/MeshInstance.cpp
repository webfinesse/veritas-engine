#include <vector>

#include "MeshInstance.h"

#include "BufferIndicies.h"
#include "Engine.h"
#include "MeshSubset.h"
#include "MeshNode.h"
#include "Skeleton.h"
#include "../VeritasEngineBase/Animation.h"
#include "../VeritasEngineBase/ResourceHandle.h"


struct VeritasEngine::MeshInstance::Impl: SmallObject<>
{
	Impl()
		: m_root{}, m_skeleton{}, m_animation{}, m_subsets{}
	{
	}

	MeshNode m_root;
	const ResourceHandle* m_skeleton;
	std::vector<Animation> m_animation;
	std::vector<MeshSubset> m_subsets;
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

VeritasEngine::MeshSubset& VeritasEngine::MeshInstance::CreateSubset()
{
	m_impl->m_subsets.emplace_back();
	return m_impl->m_subsets.back();
}

VeritasEngine::MeshSubset& VeritasEngine::MeshInstance::GetSubset(unsigned int index) const
{
	return m_impl->m_subsets[index];
}

size_t VeritasEngine::MeshInstance::GetSubsetCount() const
{
	return m_impl->m_subsets.size();
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