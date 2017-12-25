#include "ResourceManager.h"
#include "../VeritasEngineBase/FileHelper.h"

#include "IResourceLoader.h"

#include "../VeritasEngineBase/zip_file.hpp"
#include "StringHash.h"

#include <tbb/concurrent_hash_map.h>
#include "IJobManager.h"
#include "ResourceData.h"

using ResourceMap = tbb::concurrent_hash_map<VeritasEngine::ResourceHandle, VeritasEngine::ResourceData>;

struct ResourcePathParts
{
	std::string m_zipPath;
	std::string m_archivePath;
	std::string m_extension;
};

struct VeritasEngine::ResourceManager::Impl : public VeritasEngine::SmallObject<>
{
	Impl(std::vector<std::unique_ptr<IResourceLoader>>&& resourceLoaders, std::shared_ptr<IJobManager>&& jobManager)
		: m_jobManager{ std::move(jobManager) }
	{
		for(auto& loader : resourceLoaders)
		{
			m_loaders.insert(std::make_pair(loader->GetExtensionHash(), std::move(loader)));
		}
	}

	Impl(const Impl&) = delete;
	Impl(Impl&&) = delete;

	~Impl() noexcept = default;

	Impl& operator=(Impl&&) = delete;
	Impl& operator=(const Impl&) = delete;

	void SetBase(const std::string& basePath)
	{
		m_files = FileHelper::FindAllResourceFilesInDirectory(basePath);
	}

	ResourcePathParts GetResourcePathParts(const std::string& path)
	{
		ResourcePathParts parts;

		const std::string::size_type pos = path.find('/');
		if (pos != std::string::npos)
		{
			const std::string zipName(path.substr(0, pos));

			const auto item = m_files.find(zipName);

			if (item != m_files.end())
			{
				parts.m_zipPath = item->second;
				parts.m_archivePath = path.substr(pos + 1);

				const auto index = path.find_last_of('.');

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

	ResourceHandle LoadResource(ResourceManager& manager, const ResourceId& resourcePath)
	{
		const auto hash = Hash(resourcePath);
		ResourceMap::const_accessor a;
		const auto handleInserted = m_resources.insert(a, hash);		
		a.release();

		if(handleInserted)
		{
			const auto job = m_jobManager->CreateJob([&, hash, resourcePath](auto parent, auto data)
			{
				const auto parts = GetResourcePathParts(resourcePath);

				zip_file zip(parts.m_zipPath);
				auto& stream = zip.open(parts.m_archivePath);

				const auto& loader = m_loaders.find(Hash(parts.m_extension));

				if (loader != m_loaders.cend())
				{
					ResourceMap::accessor writeAccessor;
					m_resources.insert(writeAccessor, hash);

					loader->second->LoadResource(manager, parent, stream, writeAccessor->second);

					writeAccessor.release();
				}				
			});

			m_jobManager->Run(job);
			m_jobManager->Wait(job);
		}

		return hash;
	}

	ResourceHandle LoadResource(ResourceManager& manager, const ResourceId& resourcePath, Job* parentJob)
	{
		const auto hash = Hash(resourcePath);
		ResourceMap::const_accessor a;
		const auto handleInserted = m_resources.insert(a, hash);
		a.release();

		if (handleInserted)
		{
			const auto job = m_jobManager->CreateJobAsChild(parentJob, [&, hash, resourcePath](auto parent, auto data)
			{
				const auto parts = GetResourcePathParts(resourcePath);

				zip_file zip(parts.m_zipPath);
				auto& stream = zip.open(parts.m_archivePath);

				const auto& loader = m_loaders.find(Hash(parts.m_extension));

				if (loader != m_loaders.cend())
				{
					ResourceMap::accessor writeAccessor;
					m_resources.insert(writeAccessor, hash);

					loader->second->LoadResource(manager, parent, stream, writeAccessor->second);

					writeAccessor.release();
				}
			});

			m_jobManager->Run(job);
		}

		return hash;
	}

	bool GetResource(const ResourceHandle handle, GetResourceCallback&& callback) const
	{
		ResourceMap::const_accessor a;
		const auto found = m_resources.find(a, handle);

		if(found)
		{
			callback(a->second);
		}

		return found;
	}

private:
	static const std::string m_emptyZipName;

	ResourceMap m_resources{};	
	std::unordered_map<StringHash, std::unique_ptr<IResourceLoader>> m_loaders{};
	std::unordered_map<std::string, std::string> m_files;
	std::shared_ptr<IJobManager> m_jobManager;
};

const std::string VeritasEngine::ResourceManager::Impl::m_emptyZipName = "";

VeritasEngine::ResourceManager::ResourceManager(std::vector<std::unique_ptr<IResourceLoader>> resourceLoaders, std::shared_ptr<IJobManager> jobManager)
	: m_impl{ std::make_unique<Impl>(std::move(resourceLoaders), std::move(jobManager)) }
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

VeritasEngine::ResourceHandle VeritasEngine::ResourceManager::LoadResource(const ResourceId& resourcePath)
{
	return m_impl->LoadResource(*this, resourcePath);
}

VeritasEngine::ResourceHandle VeritasEngine::ResourceManager::LoadResource(const ResourceId& resourcePath, Job* parentJob)
{
	return m_impl->LoadResource(*this, resourcePath, parentJob);
}

bool VeritasEngine::ResourceManager::GetResource(const ResourceHandle handle, GetResourceCallback&& callback) const
{
	return m_impl->GetResource(handle, std::move(callback));
}
