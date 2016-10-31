#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class MeshResourceLoader : public IResourceLoader
	{
	public:
		virtual ~MeshResourceLoader() override;
		virtual void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
