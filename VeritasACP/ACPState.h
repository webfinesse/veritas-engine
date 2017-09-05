#pragma once

#include <memory>

#include "../VeritasEngineBase/Singleton.h"
#include "BoostFilesystemHelper.h"
#include <set>


namespace VeritasACP
{
	class ResourceIdGenerator;

	class ACPState : public VeritasEngine::Singleton<ACPState>
	{
		friend class VeritasEngine::Singleton<ACPState>;

	public:
		void Init(fs::path& basePath);
		ResourceIdGenerator& GetResourceIdGenerator();
		void AddAsset(fs::path& path);
		size_t GetAssetListSize();
		void ResetAssetList();
		std::set<fs::path>& GetAssetList();

	private:
		ACPState();
		~ACPState();

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
