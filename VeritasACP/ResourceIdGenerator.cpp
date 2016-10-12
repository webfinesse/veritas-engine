#include "ResourceIdGenerator.h"

#include <boost/filesystem.hpp>

struct VeritasACP::ResourceIdGenerator::Impl
{
	Impl(fs::path& basePath)
		: m_basePath(basePath)
	{

	}

	~Impl()
	{

	}

	fs::path m_basePath;
};

VeritasACP::ResourceIdGenerator::ResourceIdGenerator(fs::path& basePath)
	: m_impl(std::make_unique<Impl>(basePath))
{

}

VeritasACP::ResourceIdGenerator::~ResourceIdGenerator()
{

}

VeritasEngine::ResourceId VeritasACP::ResourceIdGenerator::GenerateId(fs::path& fullPath)
{
	return fs::relative(fullPath, m_impl->m_basePath).generic_string();
}
