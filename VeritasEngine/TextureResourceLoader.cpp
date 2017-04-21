#include "TextureResourceLoader.h"
#include <istream>

#include "../VeritasEngineBase/TextureResourceLoaderImpl.h"
#include "../VeritasEngineBase/ResourceHandle.h"

VeritasEngine::TextureResourceLoader::~TextureResourceLoader() = default;

const char* VeritasEngine::TextureResourceLoader::GetExtension() const
{
	return ".dds";
}

void VeritasEngine::TextureResourceLoader::LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	TextureResourceLoaderImpl impl;

	impl.Load(data, handle);
}
