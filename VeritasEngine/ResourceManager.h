#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../VeritasEngineBase/ResourceId.h"
#include "SmallObject.h"
#include "IResourceManager.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class IJobManager;

	class ResourceManager : public SmallObject<>, public IResourceManager
	{
	public:
		ResourceManager(std::vector<std::unique_ptr<IResourceLoader>> resourceLoaders, std::shared_ptr<IJobManager> jobManager);
		ResourceManager(ResourceManager&& other) noexcept;
		~ResourceManager() override;
		ResourceManager& operator=(ResourceManager&& other) noexcept;

		void Init(const std::string& path) override;

		ResourceHandle LoadResource(const ResourceId& resourcePath) override;
		ResourceHandle LoadResource(const ResourceId& resourcePath, Job* parentJob) override;
		bool GetResource(const ResourceHandle handle, GetResourceCallback&& callback) const override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)
