#pragma once
#include <string>
#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/ResourceId.h"


namespace VeritasEngine
{
	class ResourceHandle;
	class IResourceLoader;

	class EXPORT IResourceManager
	{
	public:
		virtual ~IResourceManager() = default;

		virtual void Init(const std::string& path) = 0;

		virtual ResourceHandle* GetResource(const ResourceId& resourcePath) = 0;
	};
}
