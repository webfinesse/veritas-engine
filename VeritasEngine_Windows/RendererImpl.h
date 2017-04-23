#ifndef H_RENDERERIMPL
#define H_RENDERERIMPL

#include <memory>

#include "../VeritasEngine/IRenderer.h"

namespace VeritasEngine
{
	class DirectXState;

	class RendererImpl: public IRenderer
	{
	public:
		RendererImpl(std::shared_ptr<DirectXState> dxState);
		RendererImpl(RendererImpl&& other) noexcept;
		~RendererImpl() override;

		RendererImpl& operator=(RendererImpl&& other) noexcept;

		void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight) override;
		void Resize(unsigned int bufferWidth, unsigned int bufferHeight) override;
		void Clear() override;
		
		void RenderSubset(const MeshInstance& mesh, unsigned int subsetIndex) const override;
		void Present() override;

		float GetAspectRatio() const override;

	private:
		RendererImpl(RendererImpl& rhs) = delete;
		RendererImpl& operator=(RendererImpl& rhs) = delete;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
