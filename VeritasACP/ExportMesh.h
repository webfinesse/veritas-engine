#ifndef H_EXPORTMESH
#define H_EXPORTMESH

#include <memory>

#include "MeshExporterResult.h"

#include "BoostFilesystemHelper.h"

namespace VeritasACP
{
	class ExportMesh
	{
	public:
		ExportMesh();
		~ExportMesh();

		std::shared_ptr<MeshExporterResult> Export(fs::path& fileName);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
