#ifndef H_VERTEXBUFFERIMPL
#define H_VERTEXBUFFERIMPL

#include <memory>

namespace VeritasEngine
{
	class VertexBufferImpl
	{
	public:
		VertexBufferImpl();
		void SetData(size_t numOfElements, size_t sizeOfEachVertex, void* bufferData);
		void* GetNativeBuffer() const;
		~VertexBufferImpl();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
