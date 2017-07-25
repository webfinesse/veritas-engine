#pragma once

namespace VeritasEngine
{
	struct PassBuffer;
	struct PerObjectBufferDescription;
	
	class IAnimatedMeshShader
	{
	public:
		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
		virtual void Init() = 0;

		virtual void SetPassParameters(PassBuffer& passBuffer) = 0;
		virtual void SetPerObjectBuffer(const PerObjectBufferDescription& buffer) = 0;

		virtual ~IAnimatedMeshShader() noexcept = default;
	};
}