#pragma once

#include <memory>
#include "IResourceLoader.h"


namespace VeritasEngine
{
	class IRenderingServices;
	class ResourceManager;

	class AnimatedMeshResourceLoader : public IResourceLoader
	{
	public:
		AnimatedMeshResourceLoader(std::shared_ptr<IRenderingServices> renderingServices);
		AnimatedMeshResourceLoader(AnimatedMeshResourceLoader&&) noexcept = default;
		AnimatedMeshResourceLoader& operator=(AnimatedMeshResourceLoader&&) noexcept = default;

		~AnimatedMeshResourceLoader() override;
		const char* GetExtension() const override;
		void LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle) override;
	
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
