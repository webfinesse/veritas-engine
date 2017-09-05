#include "ACPState.h"

#include "ResourceIdGenerator.h"
#include <boost/filesystem.hpp>
#include "../Includes/miniz-cpp-master/zip_file.hpp"

#include <set>

struct VeritasACP::ACPState::Impl
{
	Impl(fs::path& basePath)
		: m_resourceGenerator(basePath), m_assetList()
	{

	}

	ResourceIdGenerator m_resourceGenerator;
	std::set<fs::path> m_assetList;
};

VeritasACP::ACPState::ACPState()
{

}

VeritasACP::ACPState::~ACPState()
{

}

void VeritasACP::ACPState::Init(fs::path& basePath)
{
	m_impl = std::make_unique<Impl>(basePath);
}

VeritasACP::ResourceIdGenerator& VeritasACP::ACPState::GetResourceIdGenerator()
{
	return m_impl->m_resourceGenerator;
}

void VeritasACP::ACPState::AddAsset(fs::path& path)
{
	m_impl->m_assetList.emplace(path);
}

size_t VeritasACP::ACPState::GetAssetListSize()
{
	return m_impl->m_assetList.size();
}

void VeritasACP::ACPState::ResetAssetList()
{
	m_impl->m_assetList.clear();
}

std::set<fs::path>& VeritasACP::ACPState::GetAssetList()
{
	return m_impl->m_assetList;
}
