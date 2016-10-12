#pragma once

#include <memory>
#include <vector>

#include "../VeritasEngineBase/Singleton.h"
#include "BoostFilesystemHelper.h"


namespace VeritasACP
{
	class ResourceIdGenerator;

	class ACPState : public VeritasEngine::Singleton<ACPState>
	{
		friend class VeritasEngine::Singleton<ACPState>;

	public:
		void Init(fs::path& basePath);
		ResourceIdGenerator& GetResourceIdGenerator();
		std::vector<fs::path>& GetAssetList();		

	private:
		ACPState();
		~ACPState();

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
