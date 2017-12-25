#ifndef H_RENDERERIMPL
#define H_RENDERERIMPL

#include <memory>

#include "../VeritasEngine/IRenderer.h"

namespace VeritasEngine
{
	class DirectXState;
	class IMeshShader;
	class IAnimatedMeshShader;

	class RendererImpl: public IRenderer
	{
	public:
		RendererImpl(std::shared_ptr<DirectXState> dxState, std::shared_ptr<IMeshShader> meshShader, std::shared_ptr<IAnimatedMeshShader> animatedMeshShader);
		RendererImpl(RendererImpl&& other) noexcept;
		~RendererImpl() override;

		RendererImpl& operator=(RendererImpl&& other) noexcept;

		void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight, std::shared_ptr<IResourceManager> resourceManager) override;
		void Resize(unsigned int bufferWidth, unsigned int bufferHeight) override;
		void Render(FrameDescription& frameDesc) override;

		float GetAspectRatio() const override;

	private:
		RendererImpl(RendererImpl& rhs) = delete;
		RendererImpl& operator=(RendererImpl& rhs) = delete;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
