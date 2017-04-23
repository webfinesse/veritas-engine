#ifndef H_MESHSHADERIMPL
#define H_MESHSHADERIMPL

#include <memory>
#include <array>

#include "../VeritasEngineBase/Light.h"
#include "../VeritasEngine/IMeshShader.h"

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

		void SetLightParameters(const std::array<VeritasEngine::Light, Light::MAX_LIGHTS>& lightsArray) override;
		void SetCameraParameters(const Float3& eyePosition, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) override;
		void SetPerObjectBuffer(const PerObjectBufferRef& buffer) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
