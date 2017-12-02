#include "MaterialResourceLoader.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include "../VeritasEngineBase/Material.h"
#include "../VeritasEngineBase/ResourceHandle.h"

#include "ResourceManager.h"
#include "../VeritasEngineBase/MaterialInstance.h"

constexpr char extension[] = ".mat";

VeritasEngine::MaterialResourceLoader::MaterialResourceLoader()
{

}

VeritasEngine::MaterialResourceLoader::~MaterialResourceLoader() = default;

const char* VeritasEngine::MaterialResourceLoader::GetExtension() const
{
	return extension;
}

VeritasEngine::StringHash VeritasEngine::MaterialResourceLoader::GetExtensionHash() const
{
	constexpr auto hash = CompileTimeHash(extension);
	return hash;
}

void VeritasEngine::MaterialResourceLoader::LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	cereal::BinaryInputArchive archive(data);

	Material material;
	archive(material);

	MaterialInstance materialInstance{ material };

	if(material.DiffuseMap.length() > 0)
	{
		materialInstance.DiffuseMap = manager.GetResource(material.DiffuseMap);
	}

	if (material.NormalMap.length() > 0)
	{
		materialInstance.NormalMap = manager.GetResource(material.NormalMap);
	}

	if (material.SpecularMap.length() > 0)
	{
		materialInstance.SpecularMap = manager.GetResource(material.SpecularMap);
	}

	if(material.TransparentMap.length() > 0)
	{
		materialInstance.TransparentMap = manager.GetResource(material.TransparentMap);
	}

	handle.SetData(std::move(materialInstance));
}
