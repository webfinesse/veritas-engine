#include "MaterialResourceLoader.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include "../VeritasEngineBase/Material.h"

#include "ResourceManager.h"
#include "../VeritasEngineBase/MaterialInstance.h"

#include "ResourceData.h"

constexpr char extension[] = ".mat";

const char* VeritasEngine::MaterialResourceLoader::GetExtension() const
{
	return extension;
}

VeritasEngine::StringHash VeritasEngine::MaterialResourceLoader::GetExtensionHash() const
{
	constexpr auto hash = CompileTimeHash(extension);
	return hash;
}

void VeritasEngine::MaterialResourceLoader::LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle)
{
	cereal::BinaryInputArchive archive(data);

	Material material;
	archive(material);

	MaterialInstance materialInstance{ material };

	if(material.DiffuseMap.length() > 0)
	{
		materialInstance.DiffuseMap = manager.LoadResource(material.DiffuseMap, parentJob);
	}

	if (material.NormalMap.length() > 0)
	{
		materialInstance.NormalMap = manager.LoadResource(material.NormalMap, parentJob);
	}

	if (material.SpecularMap.length() > 0)
	{
		materialInstance.SpecularMap = manager.LoadResource(material.SpecularMap, parentJob);
	}

	if(material.TransparentMap.length() > 0)
	{
		materialInstance.TransparentMap = manager.LoadResource(material.TransparentMap, parentJob);
	}

	handle.SetData(std::move(materialInstance));
}
