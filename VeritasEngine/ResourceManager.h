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
	class ResourceManager : public SmallObject<>, public IResourceManager
	{
	public:
		ResourceManager(std::vector<std::unique_ptr<IResourceLoader>> resourceLoaders);
		ResourceManager(ResourceManager&& other) noexcept;
		~ResourceManager() override;
		ResourceManager& operator=(ResourceManager&& other) noexcept;

		void Init(const std::string& path) override;

		ResourceHandle* GetResource(const ResourceId& resourcePath) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)
