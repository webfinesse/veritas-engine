#include "MaterialInstance.h"
#include "../VeritasEngineBase/Material.h"

VeritasEngine::MaterialInstance::MaterialInstance(VeritasEngine::Material& material)
	: Material(material), DiffuseMap(nullptr), NormalMap(nullptr), SpecularMap(nullptr)
{
}

VeritasEngine::GraphicsCardMaterial::GraphicsCardMaterial(Material& material)
	: Ambient(material.Ambient), Diffuse(material.Diffuse), Specular(material.Specular), Emissive(material.Emissive)
{

}
