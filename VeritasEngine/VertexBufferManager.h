#ifndef H_VERTEXBUFFERMANAGER
#define H_VERTEXBUFFERMANAGER

#include "VertexTypeHandle.h"
#include "SmallObject.h"
#include <memory>


namespace VeritasEngine
{
	class VertexBuffer;

	class VertexBufferManager: public SmallObject<>
	{
	public:
		VertexBufferManager();
		~VertexBufferManager();

		void RegisterVertexFormat(VertexTypeHandle handle, size_t sizeOfVertex);
		std::shared_ptr<VeritasEngine::VertexBuffer> GetBuffer(VertexTypeHandle handle) const;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif