#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class SkeletonResourceLoader: public IResourceLoader
	{
	public:
		const char* GetExtension() const override;
		StringHash GetExtensionHash() const override;
		void LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle) override;
	};
}


