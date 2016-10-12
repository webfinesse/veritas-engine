#include "TextureResourceLoader.h"
#include <istream>

#include "../VeritasEngineBase/TextureResourceLoaderImpl.h"

VeritasEngine::TextureResourceLoader::~TextureResourceLoader() = default;

void VeritasEngine::TextureResourceLoader::LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	TextureResourceLoaderImpl impl;

	impl.Load(data, handle);
}
