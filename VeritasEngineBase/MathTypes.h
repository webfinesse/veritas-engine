#ifndef H_MATHTYPES
#define H_MATHTYPES

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace VeritasEngine
{
	using Matrix4x4 = glm::mat4x4;

	using Float2 = glm::vec2;
	using Float3 = glm::vec3;
	using Float4 = glm::vec4;
	using Quaternion = glm::quat;

	class MathHelpers
	{
	public: 
		template <typename... Args>
		static auto Transpose(Args&&... args) -> decltype(glm::transpose(std::forward<Args>(args)...)) 
		{
			return glm::transpose(std::forward<Args>(args)...);
		}

		template <typename... Args>
		static auto Inverse(Args&&... args) -> decltype(glm::inverse(std::forward<Args>(args)...)) 
		{
			return glm::inverse(std::forward<Args>(args)...);
		}

		static Matrix4x4 CreateLookAtMatrix(const Float3& eye, const Float3& target, const Float3& up) {
			return glm::lookAtLH(eye, target, up);
		}

		static Matrix4x4 CreatePerspectiveMatrix(float& fov, float& aspect, float& zNear, float& zFar)
		{
			return glm::perspectiveLH(fov, aspect, zNear, zFar);
		}

		static Matrix4x4 CreateRotationRollPitchYawMatrix(float roll, float pitch, float yaw)
		{
			return glm::eulerAngleYXZ(yaw, pitch, roll);
		}

		static float ToDegrees(float& radians)
		{
			return glm::degrees(radians);
		}

		static float ToRadians(float& degrees)
		{
			return glm::radians(degrees);
		}
	};
}

namespace cereal
{
	template<class Archive>
	void serialize(Archive & archive, VeritasEngine::Float2& m)
	{
		archive(m.x, m.y);
	}

	template<class Archive>
	void serialize(Archive & archive, VeritasEngine::Float3& m)
	{
		archive(m.x, m.y, m.z);
	}

	template<class Archive>
	void serialize(Archive& archive, VeritasEngine::Float4& m)
	{
		archive(m.x, m.y, m.z, m.w);
	}

	template<class Archive>
	void serialize(Archive& archive, VeritasEngine::Matrix4x4& m)
	{
		archive(m[0], m[1], m[2], m[3]);
	}
}

#endif
