#pragma once

#include <memory>

#include "IAnimatedMeshShader.h"

namespace VeritasEngine
{
	class DirectXState;
	class IResourceManager;

	class AnimatedMeshShaderImpl: public IAnimatedMeshShader
	{
	public:
		AnimatedMeshShaderImpl(std::shared_ptr<DirectXState> dxState);
		AnimatedMeshShaderImpl(AnimatedMeshShaderImpl&& other) noexcept;
		~AnimatedMeshShaderImpl() noexcept override;

		AnimatedMeshShaderImpl& operator=(AnimatedMeshShaderImpl&& other) noexcept;

		void Activate() override;
		void Deactivate() override;
		void Init(std::shared_ptr<IResourceManager> resourceManager) override;

		void SetPassParameters(PassBuffer& passBuffer) override;
		void SetPerObjectBuffer(const PerAnimatedObjectBufferDescription& buffer) override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}