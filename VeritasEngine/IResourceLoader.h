#pragma once

#include <iosfwd>

namespace VeritasEngine
{
	class ResourceManager;
	class ResourceHandle;

	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader() = default;

		virtual void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) = 0;
	};
}
