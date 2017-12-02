#include "TextureResourceLoader.h"
#include <istream>

#include "ITextureLoader.h"
#include "../VeritasEngineBase/ResourceHandle.h"

constexpr char extension[] = ".dds";

VeritasEngine::TextureResourceLoader::TextureResourceLoader(std::shared_ptr<ITextureLoader> textureLoader)
	: m_textureLoader { std::move(textureLoader) }
{
}

VeritasEngine::TextureResourceLoader::~TextureResourceLoader() noexcept = default;

const char* VeritasEngine::TextureResourceLoader::GetExtension() const
{
	return extension;
}

VeritasEngine::StringHash VeritasEngine::TextureResourceLoader::GetExtensionHash() const
{
	constexpr auto hash = CompileTimeHash(extension);
	return hash;
}

void VeritasEngine::TextureResourceLoader::LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	m_textureLoader->Load(data, handle);
}
