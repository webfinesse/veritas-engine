#pragma once

#include "IResourceLoader.h"
#include <memory>

namespace VeritasEngine
{
	class IJobManager;

	class MaterialResourceLoader: public IResourceLoader
	{
	public:
		MaterialResourceLoader() = default;
		~MaterialResourceLoader() override = default;

		const char* GetExtension() const override;
		StringHash GetExtensionHash() const override;
		void LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle) override;
	};
}
