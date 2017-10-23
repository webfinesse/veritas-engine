#pragma once

#include <memory>

namespace VeritasEngine
{
	class IVertexBuffer;

	class IVertexBufferFactory
	{
	public:
		virtual ~IVertexBufferFactory() = default;

		virtual std::shared_ptr<IVertexBuffer> Create(const std::size_t sizeOfVertex) = 0;
	};
}