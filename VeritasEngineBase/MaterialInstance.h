#pragma once

#include "../VeritasEngineBase/MathTypes.h"

namespace VeritasEngine
{
	class ResourceHandle;
	struct Material;

	struct GraphicsCardMaterial
	{
		GraphicsCardMaterial() = default;
		GraphicsCardMaterial(const Material& material);

		Float4 Ambient;
		Float4 Diffuse;
		Float4 Specular;
		Float4 Emissive;
		float SpecularPower;
		Float3 pad;
	};

	struct MaterialInstance
	{
		MaterialInstance(const Material& material);

		GraphicsCardMaterial Material;
		ResourceHandle* DiffuseMap;
		ResourceHandle* NormalMap;
		ResourceHandle* SpecularMap;
	};
}
