#ifndef H_INDEXBUFFERMANAGER
#define H_INDEXBUFFERMANAGER

#include <memory>

#include "../VeritasEngineBase/SmallObject.h"

namespace VeritasEngine
{
	class IndexBuffer;

	class IndexBufferManager : public SmallObject<> 
	{
	public:
		IndexBufferManager();
		~IndexBufferManager();

		IndexBuffer& GetBuffer();

	private:

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
