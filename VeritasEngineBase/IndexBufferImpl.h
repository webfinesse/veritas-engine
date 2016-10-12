#ifndef H_INDEXBUFFERImpl
#define H_INDEXBUFFERImpl

#include <memory>

namespace VeritasEngine
{
	class IndexBufferImpl
	{
	public:
		IndexBufferImpl();
		void SetData(unsigned int numOfElements, void* bufferData);
		void* GetNativeBuffer() const;
		~IndexBufferImpl();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
