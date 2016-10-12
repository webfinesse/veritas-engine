#include <vector>

#include "MeshInstance.h"

#include "BufferIndicies.h"
#include "Engine.h"
#include "RenderingServices.h"
#include "Vertex.h"
#include "MeshSubset.h"
#include "MeshNode.h"


struct VeritasEngine::MeshInstance::Impl: public VeritasEngine::SmallObject<>
{
	Impl()
		: m_root{}, m_subsets{}
	{
	}

	MeshNode m_root;
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