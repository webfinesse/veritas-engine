#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class ResourceManager;

	class AnimatedMeshResourceLoader : public IResourceLoader
	{
	public:
		~AnimatedMeshResourceLoader() override;
		void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
