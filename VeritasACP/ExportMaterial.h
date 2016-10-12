#pragma once

#include <memory>

#include "MeshExporterResult.h"

#include "BoostFilesystemHelper.h"
#include "../VeritasEngineBase/ResourceId.h"

#include "../Includes/assimp/include/assimp/scene.h"

namespace VeritasACP
{
	class ExportMaterial
	{
	public:
		ExportMaterial();
		~ExportMaterial();

		VeritasEngine::ResourceId Export(fs::path& basePath, const aiScene * scene, aiMesh * mesh);
	};
}
