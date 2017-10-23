#ifndef H_VERTEXBUFFERIMPL
#define H_VERTEXBUFFERIMPL

#include <memory>

#include "../VeritasEngine/IVertexBuffer.h"

namespace VeritasEngine
{
	class DirectXState;

	class VertexBufferImpl: public IVertexBuffer
	{
	public:
		VertexBufferImpl(std::shared_ptr<DirectXState> dxState, const size_t vertexSize);
		~VertexBufferImpl() override;

		BufferIndicies AddVerticies(unsigned char* verticies, const size_t numVerticies) override;
		size_t GetVertexSize() const override;
		void* GetNativeBuffer() const override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
