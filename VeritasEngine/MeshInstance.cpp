#include <vector>

#include "MeshInstance.h"

#include "BufferIndicies.h"
#include "Engine.h"
#include "MeshSubset.h"
#include "MeshNode.h"
#include "Skeleton.h"


struct VeritasEngine::MeshInstance::Impl: SmallObject<>
{
	Impl()
		: m_root{}, m_skeleton{}, m_subsets{}
	{
	}

	MeshNode m_root;
	Skeleton m_skeleton;
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

unsigned int VeritasEngine::MeshInstance::GetSubsetCount() const
{
	return m_impl->m_subsets.size();
}

void VeritasEngine::MeshInstance::SetSkeleton(const Skeleton& skeleton)
{
	m_impl->m_skeleton = skeleton;
}

const VeritasEngine::Skeleton& VeritasEngine::MeshInstance::GetSkeleton() const
{
	return m_impl->m_skeleton;
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