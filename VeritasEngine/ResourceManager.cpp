#include "ResourceManager.h"
#include "../VeritasEngineBase/FileHelper.h"

#include "IResourceLoader.h"

#include "../VeritasEngineBase/zip_file.hpp"
#include "../VeritasEngineBase/ResourceHandle.h"
#include "MeshResourceLoader.h"
#include "MaterialResourceLoader.h"
#include "TextureResourceLoader.h"
#include "SkeletonResourceLoader.h"
#include "AnimatedMeshResourceLoader.h"
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

	std::unordered_map<VeritasEngine::ResourceId, ResourceHandle> m_resources;
	static const std::string m_emptyZipName;
	static const AssocVector<StringHash, std::shared_ptr<IResourceLoader>> m_loaders;

private:
	std::unordered_map<std::string, std::string> m_files;
};

const std::string VeritasEngine::ResourceManager::Impl::m_emptyZipName = "";
const AssocVector<VeritasEngine::StringHash, std::shared_ptr<VeritasEngine::IResourceLoader>> VeritasEngine::ResourceManager::Impl::m_loaders 
{
	{ VESTRINGHASH(".vem"), std::make_shared<MeshResourceLoader>() },
	{ VESTRINGHASH(".mat"), std::make_shared<MaterialResourceLoader>() },
	{ VESTRINGHASH(".dds"), std::make_shared<TextureResourceLoader>() },
	{ VESTRINGHASH(".vesh"), std::make_shared<SkeletonResourceLoader>() },
	{ VESTRINGHASH(".veam"), std::make_shared<AnimatedMeshResourceLoader>() }
};

VeritasEngine::ResourceManager::ResourceManager()
	: m_impl{ std::make_unique<Impl>() }
{

}

VeritasEngine::ResourceManager::~ResourceManager() = default;

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

		auto loader = Impl::m_loaders.find(Hash(parts.m_extension));

		if(loader != Impl::m_loaders.end())
		{
			m_impl->m_resources.emplace(resourcePath, ResourceHandle());

			result = &(m_impl->m_resources[resourcePath]);
			loader->second->LoadResource(*this, stream, *result);
		}
	}

	return result;
}
