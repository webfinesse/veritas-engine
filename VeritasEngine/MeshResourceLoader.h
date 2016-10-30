#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class MeshResourceLoader : public IResourceLoader
	{
	public:
		virtual ~MeshResourceLoader() override;
		virtual ResourceHandle LoadResource(ResourceManager& manager, std::istream& data) override;
	};
}
