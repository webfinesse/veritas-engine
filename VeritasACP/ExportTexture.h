#ifndef H_EXPORTTEXTURE
#define H_EXPORTTEXTURE

#include <memory>
#include "../VeritasEngineBase/ResourceId.h"

#include "BoostFilesystemHelper.h"

namespace VeritasACP
{
	struct ExportTextureResult
	{
		VeritasEngine::ResourceId ResourceName;
		unsigned int OriginalHeight;
		unsigned int OriginalWidth;
		unsigned int NewHeight;
		unsigned int NewWidth;
	};

	class ExportTexture
	{
	public:
		ExportTexture();
		~ExportTexture();

		ExportTextureResult Export(fs::path& fileName, bool isNormalMap);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
