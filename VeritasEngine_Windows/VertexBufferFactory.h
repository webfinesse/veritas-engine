#pragma once

#include "../VeritasEngine/IVertexBufferFactory.h"

namespace VeritasEngine
{
	class DirectXState;

	class VertexBufferFactory: public IVertexBufferFactory
	{
	public:
		VertexBufferFactory(std::shared_ptr<DirectXState> dxState);
		~VertexBufferFactory() override = default;

		std::shared_ptr<IVertexBuffer> Create(size_t sizeOfVertex) override;

	private:
		std::shared_ptr<DirectXState> m_dxState;
	};
}
