#pragma once

#include <memory>
#include "IResourceLoader.h"


namespace VeritasEngine
{
	class IRenderingServices;
	class ResourceManager;
	class IJobManager;
	struct Job;

	class AnimatedMeshResourceLoader : public IResourceLoader
	{
	public:
		AnimatedMeshResourceLoader(std::shared_ptr<IRenderingServices> renderingServices);
		AnimatedMeshResourceLoader(AnimatedMeshResourceLoader&&) noexcept = default;
		AnimatedMeshResourceLoader& operator=(AnimatedMeshResourceLoader&&) noexcept = default;

		~AnimatedMeshResourceLoader() override = default;
		const char* GetExtension() const override;
		StringHash GetExtensionHash() const override;
		void LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle) override;
	
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
