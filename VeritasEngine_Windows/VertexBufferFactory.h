#pragma once

#include "../VeritasEngine/IVertexBufferFactory.h"

namespace VeritasEngine
{
	class VertexBufferFactory: public IVertexBufferFactory
	{
	public:
		~VertexBufferFactory() override = default;

		std::shared_ptr<IVertexBuffer> Create(size_t sizeOfVertex) override;
	};
}
