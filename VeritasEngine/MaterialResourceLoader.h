#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class MaterialResourceLoader: public IResourceLoader
	{
	public:
		MaterialResourceLoader();
		~MaterialResourceLoader() override;

		void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
