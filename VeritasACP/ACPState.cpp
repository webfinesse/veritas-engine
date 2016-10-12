#include "ACPState.h"

#include "ResourceIdGenerator.h"
#include <boost/filesystem.hpp>
#include "../Includes/miniz-cpp-master/zip_file.hpp"

struct VeritasACP::ACPState::Impl
{
	Impl(fs::path& basePath)
		: m_resourceGenerator(basePath), m_assetList()
	{

	}

	ResourceIdGenerator m_resourceGenerator;
	std::vector<fs::path> m_assetList;
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

std::vector<fs::path>& VeritasACP::ACPState::GetAssetList()
{
	return m_impl->m_assetList;
}
