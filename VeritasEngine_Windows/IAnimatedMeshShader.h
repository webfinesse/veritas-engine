#pragma once

#include "../VeritasEngineBase/MathTypes.h"

namespace VeritasEngine
{
	struct PassBuffer;
	struct PerAnimatedObjectBufferDescription;
	class IResourceManager;
	
	class IAnimatedMeshShader
	{
	public:
		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
		virtual void Init(std::shared_ptr<IResourceManager> resourceManager) = 0;

		virtual void SetPassParameters(PassBuffer& passBuffer) = 0;
		virtual void SetPerObjectBuffer(const PerAnimatedObjectBufferDescription& buffer) = 0;

		virtual ~IAnimatedMeshShader() noexcept = default;
	};
}