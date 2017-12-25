#pragma once

#include <iosfwd>
#include "StringHash.h"

namespace VeritasEngine
{
	class IResourceManager;
	class ResourceData;
	struct Job;

	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader() = default;

		virtual const char* GetExtension() const = 0;
		virtual VeritasEngine::StringHash GetExtensionHash() const = 0;
		virtual void LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle) = 0;
	};
}
