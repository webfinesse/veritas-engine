#ifndef H_PEROBJECTBUFFER
#define H_PEROBJECTBUFFER

#include "MathTypes.h"
#include "MaterialInstance.h"

namespace VeritasEngine
{
	struct alignas(16) PerObjectBuffer
	{
		Matrix4x4 m_worldTransform; // 0 - 64
		Matrix4x4 m_worldInverseTranspose; // 64 - 128
		GraphicsCardMaterial m_material; // 128-192
		int hasDiffuseMap; // 192-224
		int hasNormalMap; // 224-256
		int hasSpecularMap; // 256 - 288
	};

	struct PerObjectBufferRef
	{
	public:
		PerObjectBufferRef(const Matrix4x4& worldTransform, const Matrix4x4& inverseTranspose, const MaterialInstance& material)
			: m_worldTransform(worldTransform), m_worldInverseTranspose(inverseTranspose), m_material(material)
		{

		}

		const Matrix4x4& m_worldTransform;
		const Matrix4x4& m_worldInverseTranspose;
		const MaterialInstance& m_material;
	};
}

#endif
