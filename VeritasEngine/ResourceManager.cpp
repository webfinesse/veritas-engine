#include "ResourceManager.h"
#include "../VeritasEngineBase/FileHelper.h"

#include "IResourceLoader.h"

#include "../VeritasEngineBase/zip_file.hpp"
#include "../VeritasEngineBase/ResourceHandle.h"
#include "StringHash.h"
#include "../Includes/AssocVector/AssocVector.hpp"


struct ResourcePathParts
{
	std::string m_zipPath;
	std::string m_archivePath;
	std::string m_extension;
};

struct VeritasEngine::ResourceManager::Impl : public VeritasEngine::SmallObject<>
{
	Impl(std::vector<std::unique_ptr<IResourceLoader>> resourceLoaders)
	{
		for(auto& loader : resourceLoaders)
		{
			m_loaders.insert(std::make_pair(VeritasEngine::Hash(loader->GetExtension()), std::move(loader)));
		}
	}

	void SetBase(const std::string& basePath)
	{
		m_files = FileHelper::FindAllResourceFilesInDirectory(basePath);
	}

	ResourcePathParts GetResourcePathParts(const std::string& path)
	{
		ResourcePathParts parts;

		std::string::size_type pos = path.find('/');
		if (pos != std::string::npos)
		{
			std::string zipName(path.substr(0, pos));

			auto item = m_files.find(zipName);

			if (item != m_files.end())
			{
				parts.m_zipPath = item->second;
				parts.m_archivePath = path.substr(pos + 1);

				auto index = path.find_last_of('.');

				parts.m_extension = path.substr(index);
			}
		}
		else
		{
			parts.m_archivePath = m_emptyZipName;
			parts.m_zipPath = m_emptyZipName;
			parts.m_extension = m_emptyZipName;
		}

		return parts;
	}

	std::unordered_map<VeritasEngine::ResourceId, ResourceHandle> m_resources{};
	static const std::string m_emptyZipName;
	std::unordered_map<StringHash, std::unique_ptr<IResourceLoader>> m_loaders{};

private:
	std::unordered_map<std::string, std::string> m_files;
};

const std::string VeritasEngine::ResourceManager::Impl::m_emptyZipName = "";

VeritasEngine::ResourceManager::ResourceManager(std::vector<std::unique_ptr<IResourceLoader>> resourceLoaders)
	: m_impl{ std::make_unique<Impl>(std::move(resourceLoaders)) }
{

}

VeritasEngine::ResourceManager::ResourceManager(ResourceManager&& other) noexcept
	: m_impl { std::move(other.m_impl) }
{

}

VeritasEngine::ResourceManager::~ResourceManager() = default;

VeritasEngine::ResourceManager& VeritasEngine::ResourceManager::operator=(ResourceManager&& other) noexcept
{
	if(this != &other)
	{
		this->m_impl = std::move(other.m_impl);
	}

	return *this;
}

void VeritasEngine::ResourceManager::Init(const std::string& path)
{
	m_impl->SetBase(path);
}

VeritasEngine::ResourceHandle* VeritasEngine::ResourceManager::GetResource(const ResourceId& resourcePath)
{
	auto resourceIter = m_impl->m_resources.find(resourcePath);
	ResourceHandle* result = nullptr;

	if (resourceIter != m_impl->m_resources.end()) 
	{
		result = &resourceIter->second;
	}
	else
	{		
		auto parts = m_impl->GetResourcePathParts(resourcePath);

		zip_file zip(parts.m_zipPath);
		auto& stream = zip.open(parts.m_archivePath);

		const auto& loader = m_impl->m_loaders.find(Hash(parts.m_extension));

		if(loader != m_impl->m_loaders.cend())
		{
			m_impl->m_resources.emplace(resourcePath, ResourceHandle());

			result = &(m_impl->m_resources[resourcePath]);
			loader->second->LoadResource(*this, stream, *result);
		}
	}

	return result;
}
