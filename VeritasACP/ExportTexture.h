#ifndef H_EXPORTTEXTURE
#define H_EXPORTTEXTURE

#include <memory>
#include "../VeritasEngineBase/ResourceId.h"

#include "BoostFilesystemHelper.h"

namespace VeritasACP
{
	class ExportTexture
	{
	public:
		ExportTexture();
		~ExportTexture();

		VeritasEngine::ResourceId Export(fs::path& fileName);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
