#include "TextureResourceLoader.h"
#include <istream>

#include "../VeritasEngineBase/TextureResourceLoaderImpl.h"
#include "../VeritasEngineBase/ResourceHandle.h"

VeritasEngine::TextureResourceLoader::~TextureResourceLoader() = default;

VeritasEngine::ResourceHandle VeritasEngine::TextureResourceLoader::LoadResource(ResourceManager& manager, std::istream& data)
{
	TextureResourceLoaderImpl impl;

	ResourceHandle handle;
	impl.Load(data, handle);

	return handle;
}
