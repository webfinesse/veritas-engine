#ifndef H_MESHSHADER
#define H_MESHSHADER

#include <memory>
#include "IMeshShader.h"
#include "../VeritasEngineBase/Light.h"

namespace VeritasEngine
{
	class MeshShader: public IMeshShader
	{
	public:
		MeshShader();
		~MeshShader() override;

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
