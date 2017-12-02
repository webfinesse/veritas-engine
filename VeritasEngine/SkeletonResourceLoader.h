#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class SkeletonResourceLoader: public IResourceLoader
	{
	public:
		SkeletonResourceLoader();
		~SkeletonResourceLoader() override;

		const char* GetExtension() const override;
		StringHash GetExtensionHash() const override;
		void LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}


