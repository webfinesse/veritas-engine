#pragma once

#include "MeshExporterResult.h"

#include "BoostFilesystemHelper.h"
#include "../VeritasEngineBase/ResourceId.h"

#include <assimp/scene.h>

namespace VeritasACP
{
	struct ExportMaterialResult
	{
		VeritasEngine::ResourceId ResourceName{};
		unsigned int OriginalTextureWidth{};
		unsigned int OriginalTextureHeight{};
		unsigned int NewTextureWidth{};
		unsigned int NewTextureHeight{};
	};

	class ExportMaterial
	{
	public:
		ExportMaterial();
		~ExportMaterial();

		ExportMaterialResult Export(fs::path& basePath, const aiScene * scene, aiMesh * mesh);
	};
}
