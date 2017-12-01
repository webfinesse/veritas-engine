#ifndef H_PEROBJECTBUFFER
#define H_PEROBJECTBUFFER

#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngineBase/MaterialInstance.h"

namespace VeritasEngine
{
	enum ShaderFlags
	{
		ShaderFlags_None = 0,
		ShaderFlags_HasDiffuseMap = 1,
		ShaderFlags_HasNormalMap = 2,
		ShaderFlags_HasSpecularMap = 4,
		ShaderFlags_HasTransparancyMap = 8,
	};

	inline ShaderFlags operator|(ShaderFlags a, ShaderFlags b)
	{
		return static_cast<ShaderFlags>(static_cast<int>(a) | static_cast<int>(b));
	}

	struct PerObjectBuffer
	{
		PerObjectBuffer() = default;
		PerObjectBuffer(Matrix4x4& worldTransform, Matrix4x4& worldInverseTranspose, GraphicsCardMaterial& material, ShaderFlags flags)
			: WorldTransform{worldTransform}, WorldInverseTranspose{worldInverseTranspose}, Material{material}, ShaderFlags{ flags }
		{
			
		}

		Matrix4x4 WorldTransform; // 0 - 64 bytes
		Matrix4x4 WorldInverseTranspose; // 64 - 128 bytes
		GraphicsCardMaterial Material; // 128 - 208 bytes
		int ShaderFlags; // 208 - 212 bytes
		char pad[12]; // 212 - 224 bytes
	};
}

#endif
