#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class MaterialResourceLoader: public IResourceLoader
	{
	public:
		MaterialResourceLoader();
		~MaterialResourceLoader() override;

		const char* GetExtension() const override;
		void LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
