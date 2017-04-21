#ifndef H_RENDERERIMPL
#define H_RENDERERIMPL

#include <memory>

namespace VeritasEngine
{
	class RendererImpl
	{
	public:
		void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight);
		void Resize(unsigned int bufferWidth, unsigned int bufferHeight);
		void Clear();
		
		void SetVertexBuffer(void* buffer, const unsigned int strides[], const unsigned int offsets[]);
		void SetIndexBuffer(void* buffer);
		void DrawIndexed(size_t indexCount, size_t indexOffset, size_t baseVertexIndex);
		void Present();

		RendererImpl();
		~RendererImpl();

	private:
		RendererImpl(RendererImpl& rhs) = delete;
		RendererImpl& operator=(RendererImpl& rhs) = delete;

		RendererImpl(RendererImpl&& rhs) = delete;
		RendererImpl& operator=(RendererImpl&& rhs) = delete;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
