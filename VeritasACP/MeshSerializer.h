#pragma once

#include "BoostFilesystemHelper.h"

namespace VeritasACP {
	struct MeshExporterResult;
	

	class MeshSerializer
	{
	public:
		MeshSerializer();

		void Serialize(MeshExporterResult& meshInfo, fs::path& path) const;
	};
}



