#pragma once
#include <string>
#include <functional>

#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/ResourceId.h"

namespace VeritasEngine
{
	class IResourceLoader;
	class ResourceData;
	struct Job;
	using GetResourceCallback = std::function<void(const ResourceData&)>;

	class EXPORT IResourceManager
	{
	public:
		virtual ~IResourceManager() = default;

		virtual void Init(const std::string& path) = 0;

		virtual ResourceHandle LoadResource(const ResourceId& resourcePath) = 0;
		virtual ResourceHandle LoadResource(const ResourceId& resourcePath, Job* parentJob) = 0;
		virtual bool GetResource(const ResourceHandle handle, GetResourceCallback&& callback) const = 0;
	};
}
