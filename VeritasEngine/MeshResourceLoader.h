#pragma once
#include <memory>

#include "IResourceLoader.h"


namespace VeritasEngine
{
	class IRenderingServices;

	class MeshResourceLoader : public IResourceLoader
	{
	public:
		MeshResourceLoader(std::shared_ptr<IRenderingServices> renderingServices);
		MeshResourceLoader(MeshResourceLoader&&) noexcept = default;
		MeshResourceLoader& operator=(MeshResourceLoader&&) noexcept = default;

		~MeshResourceLoader() override;
		const char* GetExtension() const override;
		StringHash GetExtensionHash() const override;
		void LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
