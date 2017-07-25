#pragma once

#include <memory>

#include "IAnimatedMeshShader.h"

namespace VeritasEngine
{
	class DirectXState;

	class AnimatedMeshShaderImpl: public IAnimatedMeshShader
	{
	public:
		AnimatedMeshShaderImpl(std::shared_ptr<DirectXState> dxState);
		AnimatedMeshShaderImpl(AnimatedMeshShaderImpl&& other) noexcept;
		~AnimatedMeshShaderImpl() noexcept override;

		AnimatedMeshShaderImpl& operator=(AnimatedMeshShaderImpl&& other) noexcept;

		void Activate() override;
		void Deactivate() override;
		void Init() override;

		void SetPassParameters(PassBuffer& passBuffer) override;
		void SetPerObjectBuffer(const PerObjectBufferDescription& buffer) override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}