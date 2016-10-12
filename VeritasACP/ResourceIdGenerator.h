#ifndef H_RESOURCEIDGENERATOR
#define H_RESOURCEIDGENERATOR

#include <memory>

#include "../VeritasEngineBase/ResourceId.h"

#include "BoostFilesystemHelper.h"

namespace VeritasACP
{
	class ResourceIdGenerator
	{
	public:
		ResourceIdGenerator(fs::path& basePath);
		~ResourceIdGenerator();

		VeritasEngine::ResourceId GenerateId(fs::path& fullPath);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}


#endif
