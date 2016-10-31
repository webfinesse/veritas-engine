#include "MaterialResourceLoader.h"

#include "../Includes/cereal-1.1.2/include/cereal/cereal.hpp"
#include "../Includes/cereal-1.1.2/include/cereal/archives/binary.hpp"

#include "../VeritasEngineBase/Material.h"
#include "../VeritasEngineBase/ResourceHandle.h"

#include "ResourceManager.h"
#include "../VeritasEngineBase/MaterialInstance.h"

VeritasEngine::MaterialResourceLoader::MaterialResourceLoader()
{

}

VeritasEngine::MaterialResourceLoader::~MaterialResourceLoader() = default;


void VeritasEngine::MaterialResourceLoader::LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	cereal::BinaryInputArchive archive(data);

	Material material;
	archive(material);

	MaterialInstance materialInstance{ material };

	if(material.DiffuseMap.length() > 0)
	{
		auto diffuse = manager.GetResource(material.DiffuseMap);
		materialInstance.DiffuseMap = diffuse;
	}

	if (material.NormalMap.length() > 0)
	{
		auto normal = manager.GetResource(material.NormalMap);
		materialInstance.NormalMap = normal;
	}

	if (material.SpecularMap.length() > 0)
	{
		auto specular = manager.GetResource(material.SpecularMap);
		materialInstance.SpecularMap = specular;
	}

	handle.SetData(std::move(materialInstance));
}
