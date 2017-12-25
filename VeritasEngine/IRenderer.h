#pragma once

namespace VeritasEngine
{
	class FrameDescription;
	class IResourceManager;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight, std::shared_ptr<IResourceManager> resourceManager) = 0;
		virtual void Resize(unsigned int bufferWidth, unsigned int bufferHeight) = 0;
		virtual void Render(FrameDescription& frameDesc) = 0;

		virtual float GetAspectRatio() const = 0;
	};
}
