#ifndef H_INDEXBUFFERImpl
#define H_INDEXBUFFERImpl

#include <memory>

#include "../VeritasEngine/IIndexBuffer.h"

namespace VeritasEngine
{
	class DirectXState;

	class IndexBufferImpl: public IIndexBuffer
	{
	public:
		IndexBufferImpl(std::shared_ptr<DirectXState> dxState);
		IndexBufferImpl(IndexBufferImpl&& other) noexcept;
		~IndexBufferImpl();

		IndexBufferImpl& operator=(IndexBufferImpl&& other) noexcept;

		BufferIndicies AddIndicies(unsigned int* indicies, size_t numOfIndicies) override;
		void* GetNativeBuffer() const override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
