#pragma once

#include <memory>
#include "IResourceLoader.h"

namespace VeritasEngine
{
	class ITextureLoader;

	class TextureResourceLoader : public IResourceLoader
	{
	public:
		TextureResourceLoader(std::shared_ptr<ITextureLoader> textureLoader);
		~TextureResourceLoader() noexcept override;

		const char* GetExtension() const override;
		StringHash GetExtensionHash() const override;
		void LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle) override;
	private:
		std::shared_ptr<ITextureLoader> m_textureLoader;
	};
}
