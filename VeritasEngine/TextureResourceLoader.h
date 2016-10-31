#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class TextureResourceLoader : public IResourceLoader
	{
	public:
		virtual ~TextureResourceLoader() override;

		virtual void LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
