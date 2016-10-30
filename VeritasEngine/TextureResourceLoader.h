#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class TextureResourceLoader : public IResourceLoader
	{
	public:
		virtual ~TextureResourceLoader() override;

		virtual ResourceHandle LoadResource(ResourceManager& manager, std::istream& data) override;
	};
}
