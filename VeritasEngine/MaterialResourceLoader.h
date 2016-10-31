#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class MaterialResourceLoader: public IResourceLoader
	{
	public:
		MaterialResourceLoader();
		virtual ~MaterialResourceLoader() override;

		virtual void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
