#pragma once

#include <iosfwd>

namespace VeritasEngine
{
	class IResourceManager;
	class ResourceHandle;

	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader() = default;

		virtual const char* GetExtension() const = 0;
		virtual void LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle) = 0;
	};
}
