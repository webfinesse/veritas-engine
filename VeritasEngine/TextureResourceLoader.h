#pragma once

#include "IResourceLoader.h"

namespace VeritasEngine
{
	class TextureResourceLoader : public IResourceLoader
	{
	public:
		virtual ~TextureResourceLoader() override;

		const char* GetExtension() const override;
		virtual void LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	};
}
