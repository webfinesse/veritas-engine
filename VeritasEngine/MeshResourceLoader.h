#pragma once
#include <memory>

#include "IResourceLoader.h"


namespace VeritasEngine
{
	class IRenderingServices;
	class IJobManager;

	class MeshResourceLoader : public IResourceLoader
	{
	public:
		MeshResourceLoader(std::shared_ptr<IRenderingServices> renderingServices);
		MeshResourceLoader(MeshResourceLoader&&) noexcept = default;
		MeshResourceLoader& operator=(MeshResourceLoader&&) noexcept = default;

		~MeshResourceLoader() override = default;

		const char* GetExtension() const override;
		StringHash GetExtensionHash() const override;
		void LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
