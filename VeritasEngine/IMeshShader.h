#ifndef H_IMESHSHADER
#define H_IMESHSHADER

#include <vector>

#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngineBase/Light.h"

namespace VeritasEngine
{
	struct PerObjectBufferRef;

	class IMeshShader
	{
	public:
		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
		virtual void Init() = 0;

		virtual void SetLightParameters(const std::array<VeritasEngine::Light, Light::MAX_LIGHTS>& lightsArray) = 0;
		virtual void SetCameraParameters(const Float3& eyePosition, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) = 0;
		virtual void SetPerObjectBuffer(const PerObjectBufferRef& buffer) = 0;

		virtual ~IMeshShader() = default;
	};
}

#endif
