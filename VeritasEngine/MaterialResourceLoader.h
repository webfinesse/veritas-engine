#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class MaterialResourceLoader: public IResourceLoader
	{
	public:
		MaterialResourceLoader();
		virtual ~MaterialResourceLoader() override;

		virtual ResourceHandle LoadResource(ResourceManager& manager, std::istream& data) override;
	};
}
