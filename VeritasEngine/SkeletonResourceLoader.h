#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class SkeletonResourceLoader: public IResourceLoader
	{
	public:
		SkeletonResourceLoader();
		~SkeletonResourceLoader() override;

		void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}


