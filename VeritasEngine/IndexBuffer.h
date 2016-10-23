#ifndef H_INDEXBUFFER
#define H_INDEXBUFFER

#include <memory>

#include "SmallObject.h"

namespace VeritasEngine
{
	class BufferIndicies;

	class IndexBuffer: public SmallObject<>
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		BufferIndicies AddIndicies(unsigned int* indicies, unsigned int numOfIndicies);

		void* GetNativeBuffer();
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif