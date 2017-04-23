#pragma once

namespace VeritasEngine
{
	class MeshInstance;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight) = 0;
		virtual void Resize(unsigned int bufferWidth, unsigned int bufferHeight) = 0;
		virtual void Clear() = 0;
		virtual void RenderSubset(const MeshInstance& mesh, unsigned int subsetIndex) const = 0;
		virtual void Present() = 0;

		virtual float GetAspectRatio() const = 0;
	};
}
