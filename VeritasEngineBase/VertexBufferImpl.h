#ifndef H_VERTEXBUFFERIMPL
#define H_VERTEXBUFFERIMPL

#include <memory>

namespace VeritasEngine
{
	class VertexBufferImpl
	{
	public:
		VertexBufferImpl();
		void SetData(unsigned int numOfElements, unsigned int sizeOfEachVertex, void* bufferData);
		void* GetNativeBuffer() const;
		~VertexBufferImpl();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
