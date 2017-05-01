#ifndef H_MESHSHADERIMPL
#define H_MESHSHADERIMPL

#include <memory>

#include "IMeshShader.h"

namespace VeritasEngine
{
	struct PerObjectBufferRef;
	class DirectXState;

	class MeshShaderImpl: public IMeshShader
	{
	public:
		MeshShaderImpl(std::shared_ptr<DirectXState> dxState);
		MeshShaderImpl(MeshShaderImpl&& other) noexcept;
		~MeshShaderImpl() noexcept override;

		MeshShaderImpl& operator=(MeshShaderImpl&& other) noexcept;

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

#endif
