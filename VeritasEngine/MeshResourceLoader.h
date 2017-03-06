#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class MeshResourceLoader : public IResourceLoader
	{
	public:
		~MeshResourceLoader() override;
		void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
