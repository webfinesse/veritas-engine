#include "MaterialInstance.h"
#include "../VeritasEngineBase/Material.h"

VeritasEngine::MaterialInstance::MaterialInstance(const VeritasEngine::Material& material)
	: Material(material), DiffuseMap(0), NormalMap(0), SpecularMap(0)
{
}

VeritasEngine::GraphicsCardMaterial::GraphicsCardMaterial(const Material& material)
	: Ambient(material.Ambient), Diffuse(material.Diffuse), Specular(material.Specular), Emissive(material.Emissive), SpecularPower{}
{

}
