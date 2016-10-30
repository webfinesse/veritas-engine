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

		virtual ResourceHandle LoadResource(ResourceManager& manager, std::istream& data) = 0;
	};
}
