#ifndef H_MATHTYPES
#define H_MATHTYPES

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace VeritasEngine
{
	using Matrix4x4 = glm::mat4;

	using Float2 = glm::vec2;
	using Float3 = glm::vec3;
	using Float4 = glm::vec4;
	using Quaternion = glm::quat;

	constexpr float QuarterPi{ glm::quarter_pi<float>() };
	constexpr float ThreeOverTwoPi { glm::three_over_two_pi<float>() };
	constexpr float TwoPi { glm::two_pi<float>() };

	class MathHelpers
	{
	public:
		static auto Interpolate(const Float3 from, const Float3 to, float interpolationFactor)
		{
			return glm::mix(from, to, interpolationFactor);
		}

		static auto Interpolate(const Float4 from, const Float4 to, float interpolationFactor)
		{
			return glm::mix(from, to, interpolationFactor);
		}

		static auto Interpolate(const Quaternion from, const Quaternion to, float interpolationFactor)
		{
			return glm::mix(from, to, interpolationFactor);
		}

		static auto Transpose(const Matrix4x4& matrix) 
		{
			return glm::transpose(matrix);
		}

		static auto Inverse(const Matrix4x4& matrix) 
		{
			return glm::inverse(matrix);
		}

		static auto Inverse(const Quaternion& matrix)
		{
			return glm::inverse(matrix);
		}

		static auto Translate(const Matrix4x4 matrixToTranslate, const Float3 translation)
		{
			return glm::translate(matrixToTranslate, translation);
		}

		static auto Scale(const Matrix4x4 matrixToScale, const Float3 scale)
		{
			return glm::scale(matrixToScale, scale);
		}

		static auto Rotate(const Matrix4x4 matrixToRotate, const Quaternion rotation)
		{
			return matrixToRotate * glm::toMat4(rotation);
		}

		static auto CalculateSQT(const Float3 scale, const Quaternion rotation, const Float3 translation)
		{
			Matrix4x4 result{};

			result = MathHelpers::Translate(result, translation);
			result = MathHelpers::Rotate(result, rotation);
			result = MathHelpers::Scale(result, scale);			

			return result;
		}

		static Matrix4x4 CreateLookAtMatrix(const Float3& eye, const Float3& target, const Float3& up) {
			return glm::lookAtLH(eye, target, up);
		}

		static Matrix4x4 CreatePerspectiveMatrix(float fov, float aspect, float zNear, float zFar)
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

	template<class Archive>
	void serialize(Archive& archive, VeritasEngine::Quaternion& q)
	{
		archive(q.x, q.y, q.z, q.w);
	}
}

#endif
