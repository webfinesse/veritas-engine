#ifndef H_VERTEXBUFFERMANAGER
#define H_VERTEXBUFFERMANAGER

#include <memory>

#include "IVertexBufferManager.h"
#include "SmallObject.h"




namespace VeritasEngine
{
	class VertexBufferManager: public IVertexBufferManager, public SmallObject<>
	{
	public:
		VertexBufferManager();
		~VertexBufferManager() override;

		void RegisterVertexFormat(VertexTypeHandle handle, size_t sizeOfVertex) override;
		std::shared_ptr<VeritasEngine::VertexBuffer> GetBuffer(VertexTypeHandle handle) const override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif