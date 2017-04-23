#ifndef H_VERTEXBUFFERMANAGER
#define H_VERTEXBUFFERMANAGER

#include <memory>

#include "IVertexBufferManager.h"
#include "SmallObject.h"

namespace VeritasEngine
{
	class IVertexBufferFactory;

	class VertexBufferManager: public IVertexBufferManager, public SmallObject<>
	{
	public:
		VertexBufferManager(std::shared_ptr<IVertexBufferFactory> factory);
		VertexBufferManager(VertexBufferManager&& other) noexcept;
		~VertexBufferManager() override;

		VertexBufferManager& operator=(VertexBufferManager&& other) noexcept;

		void RegisterVertexFormat(VertexTypeHandle handle, size_t sizeOfVertex) override;
		IVertexBuffer* GetBuffer(VertexTypeHandle handle) const override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif