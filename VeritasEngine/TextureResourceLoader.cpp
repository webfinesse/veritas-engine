#include "TextureResourceLoader.h"
#include <istream>

#include "ITextureLoader.h"
#include "../VeritasEngineBase/ResourceHandle.h"

VeritasEngine::TextureResourceLoader::TextureResourceLoader(std::shared_ptr<ITextureLoader> textureLoader)
	: m_textureLoader { textureLoader }
{
}

VeritasEngine::TextureResourceLoader::~TextureResourceLoader() noexcept = default;

const char* VeritasEngine::TextureResourceLoader::GetExtension() const
{
	return ".dds";
}

void VeritasEngine::TextureResourceLoader::LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	m_textureLoader->Load(data, handle);
}
