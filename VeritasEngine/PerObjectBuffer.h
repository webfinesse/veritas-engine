#ifndef H_PEROBJECTBUFFER
#define H_PEROBJECTBUFFER

#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngineBase/MaterialInstance.h"

namespace VeritasEngine
{
	struct alignas(16) PerObjectBuffer
	{
		PerObjectBuffer() = default;
		PerObjectBuffer(Matrix4x4& worldTransform, Matrix4x4& worldInverseTranspose, GraphicsCardMaterial& material, int hasDiffuseMap, int hasNormalMap, int hasSpecularMap)
			: WorldTransform{worldTransform}, WorldInverseTranspose{worldInverseTranspose}, Material{material}, HasDiffuseMap{hasDiffuseMap}, HasNormalMap{hasNormalMap}, HasSpecularMap{hasSpecularMap}
		{
			
		}

		Matrix4x4 WorldTransform; // 0 - 64
		Matrix4x4 WorldInverseTranspose; // 64 - 128
		GraphicsCardMaterial Material; // 128-192
		int HasDiffuseMap; // 192-224
		int HasNormalMap; // 224-256
		int HasSpecularMap; // 256 - 288
	};
}

#endif
