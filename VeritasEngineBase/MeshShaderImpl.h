#ifndef H_MESHSHADERIMPL
#define H_MESHSHADERIMPL

#include <memory>
#include <array>

#include "MathTypes.h"
#include "Light.h"

namespace VeritasEngine
{
	struct PerObjectBufferRef;

	class MeshShaderImpl
	{
	public:
		MeshShaderImpl();
		~MeshShaderImpl();
		void Activate();
		void Deactivate();
		void Init();

		void SetLightParameters(const std::array<VeritasEngine::Light, Light::MAX_LIGHTS>& lightsArray);
		void SetCameraParameters(const Float3& eyePosition, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);
		void SetPerObjectBuffer(const PerObjectBufferRef& buffer);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
