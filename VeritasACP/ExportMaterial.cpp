#include "ExportMaterial.h"
#include "ExportTexture.h"
#include "BoostFilesystemHelper.h"
#include "ACPState.h"
#include "ResourceIdGenerator.h"

#include <string>

//#define ASSIMP_DLL

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "cereal/archives/binary.hpp"

#include <boost/filesystem.hpp>

VeritasACP::ExportMaterial::ExportMaterial()
{
}

VeritasACP::ExportMaterial::~ExportMaterial()
{
}

void AiColorToFloat4(aiColor4D& aiColor, VeritasEngine::Float4& engineColor)
{
	engineColor.r = aiColor.r;
	engineColor.g = aiColor.g;
	engineColor.b = aiColor.b;
	engineColor.a = aiColor.a;
}

VeritasACP::ExportMaterialResult VeritasACP::ExportMaterial::Export(fs::path& basePath, const aiScene * scene, aiMesh * mesh)
{
	auto* material = scene->mMaterials[mesh->mMaterialIndex];

	VeritasEngine::Material mat;

	aiString texPath;
	bool hasDiffuseMap = false;

	ExportMaterialResult result;

	if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
	{
		ExportTexture textureExporter;
		const std::string fileName(texPath.C_Str());
		auto copy = basePath;
		auto fullPath = copy.append(fileName);
		const auto textureResult = textureExporter.Export(fullPath, false);

		mat.DiffuseMap = textureResult.ResourceName;
		result.OriginalTextureHeight = textureResult.OriginalHeight;
		result.OriginalTextureWidth = textureResult.OriginalWidth;
		result.NewTextureHeight = textureResult.NewHeight;
		result.NewTextureWidth = textureResult.NewWidth;

		hasDiffuseMap = true;
	}

	if (material->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS)
	{
		ExportTexture textureExporter;
		const std::string fileName(texPath.C_Str());
		auto copy = basePath;
		auto fullPath = copy.append(fileName);
		mat.NormalMap = textureExporter.Export(fullPath, true).ResourceName;
	}

	if (material->GetTexture(aiTextureType_SPECULAR, 0, &texPath) == AI_SUCCESS)
	{
		ExportTexture textureExporter;
		const std::string fileName(texPath.C_Str());
		auto copy = basePath;
		auto fullPath = copy.append(fileName);
		mat.SpecularMap = textureExporter.Export(fullPath, false).ResourceName;
	}

	if(material->GetTexture(aiTextureType_OPACITY, 0, &texPath) == AI_SUCCESS)
	{
		ExportTexture textureExporter;
		const std::string fileName(texPath.C_Str());
		auto copy = basePath;
		auto fullPath = copy.append(fileName);
		mat.TransparentMap = textureExporter.Export(fullPath, false).ResourceName;
	}

	aiColor4D diffuse;
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
	{
		AiColorToFloat4(diffuse, mat.Diffuse);
	}
	else if(hasDiffuseMap)
	{
		mat.Diffuse = VeritasEngine::Float4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	aiColor4D ambient;
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient) == AI_SUCCESS)
	{
		AiColorToFloat4(ambient, mat.Ambient);
	}

	aiColor4D emissive;
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive) == AI_SUCCESS)
	{
		AiColorToFloat4(emissive, mat.Emissive);
	}

	aiColor4D specular;
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
	{
		AiColorToFloat4(specular, mat.Specular);
	}

	unsigned int max = 1;
	float strength;
	if (aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max) == AI_SUCCESS)
	{
		mat.Specular.a = strength;
	}

	std::string name("material");
	name += std::to_string(mesh->mMaterialIndex);
	name += ".mat";

	auto copy = basePath;
	auto outputPath = copy.append(name);

	std::ofstream file(outputPath.generic_string(), std::ios::binary);
	cereal::BinaryOutputArchive archive(file);

	archive(mat);

	VeritasACP::ACPState::Instance().AddAsset(outputPath);

	result.ResourceName = VeritasACP::ACPState::Instance().GetResourceIdGenerator().GenerateId(outputPath);

	return result;
}
