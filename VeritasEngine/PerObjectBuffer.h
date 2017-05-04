#ifndef H_PEROBJECTBUFFER
#define H_PEROBJECTBUFFER

#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngineBase/MaterialInstance.h"

namespace VeritasEngine
{
	struct PerObjectBuffer
	{
		PerObjectBuffer() = default;
		PerObjectBuffer(Matrix4x4& worldTransform, Matrix4x4& worldInverseTranspose, GraphicsCardMaterial& material, int hasDiffuseMap, int hasNormalMap, int hasSpecularMap)
			: WorldTransform{worldTransform}, WorldInverseTranspose{worldInverseTranspose}, Material{material}, HasDiffuseMap{hasDiffuseMap}, HasNormalMap{hasNormalMap}, HasSpecularMap{hasSpecularMap}
		{
			
		}

		Matrix4x4 WorldTransform;
		Matrix4x4 WorldInverseTranspose;
		GraphicsCardMaterial Material;
		int HasDiffuseMap;
		int HasNormalMap;
		int HasSpecularMap;
		char pad[4];
	};
}

#endif
